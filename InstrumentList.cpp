#include "InstrumentList.h"

#include <QDir>
#include <QDebug>
#include <QMutexLocker>

InstrumentList::InstrumentList(const QString &quoteFilePath)
{

    listIsObsolete_ = false;

    QDir dir = QDir(quoteFilePath);

    //Opens the path
    dir.setNameFilters(QStringList("*.csv"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    qDebug() << "Scanning: " << dir.path();
    QStringList fileList = dir.entryList();

    for (int rowNum=0; rowNum<fileList.count(); rowNum++)
    {
        qDebug() << "Found file (full path): " << dir.absoluteFilePath(fileList[rowNum]);
        qDebug() << "Found file: " << fileList[rowNum];

        InstrumentSelectionInfoPtr instrSelInfo(new InstrumentSelectionInfo(dir,fileList[rowNum]));

        instrumentInfo_.push_back(instrSelInfo);
    }

}


const InstrumentSelectionInfoPtr &InstrumentList::instrInfo(unsigned int instrNum) const
{
    assert(instrNum < instrumentInfo_.size());
    return instrumentInfo_[instrNum];
}


InstrumentSelectionInfoPtr InstrumentList::nextInstrumentForPatternScan()
{

    QMutexLocker lock(&instrumentInfoMutex_);

    if(listIsObsolete_)
    {
        return InstrumentSelectionInfoPtr(); // NULL Pointer
    }

    for(InstrumentSelectionInfoVector::iterator instrIter = instrumentInfo_.begin();
        instrIter != instrumentInfo_.end(); instrIter++)
    {
        if((!((*instrIter)->patternScanComplete())) && (!((*instrIter)->patternScanInProgress)))
        {
            InstrumentSelectionInfoPtr instrNeedingPatternScan = (*instrIter);
            instrNeedingPatternScan->patternScanInProgress = true;
            return instrNeedingPatternScan;
        }
    }


    return InstrumentSelectionInfoPtr(); // NULL Pointer
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
        instrInfo->scanPatternMatches();
    }
    return instrInfo;
}
