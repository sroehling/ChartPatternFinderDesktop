#include "InstrumentList.h"

#include <QDir>
#include <QDebug>
#include <QMutexLocker>

#include "InstrumentListTypes.h"
#include "InstrumentListTask.h"
#include "ReadQuotesFromFile.h"
#include "ScanPatternMatches.h"

InstrumentList::InstrumentList(const QString &quoteFilePath)
{

    listIsObsolete_ = false;

    QDir dir = QDir(quoteFilePath);

    //Opens the path
    dir.setNameFilters(QStringList("*.csv"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    qDebug() << "Scanning: " << dir.path();
    QStringList fileList = dir.entryList();

    numCSVFiles_ = fileList.count();
    numInstrsScanned_ = 0;

    for (int rowNum=0; rowNum<fileList.count(); rowNum++)
    {
        qDebug() << "Found file (full path): " << dir.absoluteFilePath(fileList[rowNum]);
        qDebug() << "Found file: " << fileList[rowNum];

        // Initially populate taskList_ with tasks for the InstrumentListWorker (worker thread)
        // to read and validate the quotes from the file. Once all the quotes have been read
        // and validated, the nextInstrListTask method will start returning tasks to scan
        // for the actual patterns.
        InstrumentListTaskPtr readQuotesTask(new ReadQuotesFromFile(dir,fileList[rowNum]));
        taskList_.push_back(readQuotesTask);

    }

}

void InstrumentList::addInstrumentSelectionInfo(const InstrumentSelectionInfoPtr &instrSelInfo)
{
    QMutexLocker lock(&instrumentInfoMutex_);

    // This method is called by the "ReadQuotesFromFile" Instrument List task. The given
    // instrSelInfo will not have it's patterns scanned, but this will suffice to place
    // the information into the list for display and selection in the InstrumentListTableView.

    instrumentInfo_.push_back(instrSelInfo);

    emit instrumentAdded((unsigned int)(instrumentInfo_.size()-1));
}


const InstrumentSelectionInfoPtr &InstrumentList::instrInfo(unsigned int instrNum) const
{
    assert(instrNum < instrumentInfo_.size());
    return instrumentInfo_[instrNum];
}

void InstrumentList::oneInstrScanComplete()
{
    QMutexLocker lock(&instrumentInfoMutex_);
    numInstrsScanned_++;
    emit instrumentScanProgressUpdated(numInstrsScanned_);
}


InstrumentListTaskPtr InstrumentList::nextInstrListTask()
{
    QMutexLocker lock(&instrumentInfoMutex_);

    if(listIsObsolete_)
    {
        // Return a NULL smart pointer - This will cause the worker threads
        // to exit gracefully.
        return InstrumentListTaskPtr(); // NULL Pointer
    }

    else if(taskList_.size() > 0)
    {
        // Return the next task for reading the files
        InstrumentListTaskPtr nextTask = taskList_.front();
        taskList_.pop_front();
        return nextTask;
    }
    else
    {
        // Search for instrument information needing a pattern scan, and create a
        // task for scanning that.

        for(InstrumentSelectionInfoVector::iterator instrIter = instrumentInfo_.begin();
            instrIter != instrumentInfo_.end(); instrIter++)
        {
            if((!((*instrIter)->patternScanComplete())) && (!((*instrIter)->patternScanInProgress)))
            {
                InstrumentSelectionInfoPtr instrNeedingPatternScan = (*instrIter);

                // There are still datasets/instruments/symbols which haven't had their
                // patterns scanned for yet. Pick the first one needing a pattern scan,
                // create and return a task to scan the patterns.
                instrNeedingPatternScan->patternScanInProgress = true;
                InstrumentListTaskPtr scanTask(new ScanPatternMatches(instrNeedingPatternScan));
                return scanTask;
            }
        }
        emit instrScanComplete();
        return InstrumentListTaskPtr(); // Nothing more to scan -- return NULL (smart) pointer
    }
}

void InstrumentList::obsoleteList()
{
    QMutexLocker lock(&instrumentInfoMutex_);

    listIsObsolete_ = true;
}

const InstrumentSelectionInfoPtr InstrumentList::instrInfoWithScannedPatterns(unsigned int instrNum)
{
    instrumentInfoMutex_.lock();

    // Since this method is used for immediately selecting an instrument and ensuring
    // the patterns are calculated, a selection must not occur on an obsolete list.
    assert(!listIsObsolete_);

    bool doScanPatterns = false;

    InstrumentSelectionInfoPtr instrInfo = this->instrInfo(instrNum);
    if(!instrInfo->patternScanComplete())
    {
        doScanPatterns = true;
        instrInfo->patternScanInProgress = true;
    }

    instrumentInfoMutex_.unlock();

    if(doScanPatterns)
    {
        ScanPatternMatches scannerTask(instrInfo);
        scannerTask.doTask(*this);
    }
    return instrInfo;
}
