#include "ReadQuotesFromFile.h"
#include <QDebug>
#include "PeriodValSegment.h"
#include "InstrumentSelectionInfo.h"
#include "InstrumentList.h"
#include <QDebug>

ReadQuotesFromFile::ReadQuotesFromFile(const QDir &directory, const QString &fileName)
    : dir_(directory), fileName_(fileName)
{
}

void ReadQuotesFromFile::doTask(const InstrumentListPtr &instrList)
{
    // The method PeriodValSegment::readFromFile will fail and throw an exception if the
    // file is not formatted correctly. If this happens, we will not call addInstrumentSelectionInfo,
    // which will in turn add it to the list of selectable instruments/symbols and scan patterns.
    try
    {
        QString instrFilePath(dir_.absoluteFilePath(fileName_));
        PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile(instrFilePath.toStdString());

        InstrumentSelectionInfoPtr instrSelectionInfo(new InstrumentSelectionInfo(dir_,fileName_,chartData));
        instrList->addInstrumentSelectionInfo(instrSelectionInfo);

    }
    catch(std::exception &readQuotesException)
    {
        qDebug() << "Error reading quotes data from CSV file: " << dir_.absoluteFilePath(fileName_)
                    << "error=" << readQuotesException.what();
    }
}
