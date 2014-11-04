#include "InstrumentSelectionInfo.h"
#include <QDir>



InstrumentSelectionInfo::InstrumentSelectionInfo(const QDir &directory, const QString &fileName,
                                                 const PeriodValSegmentPtr chartData)
    : dir_(directory),
      fileName_(fileName),
      chartData_(chartData)
{
    patternScanInProgress = false;
}



QString InstrumentSelectionInfo::instrumentName() const
{
    QString instrumentLabel(fileName_);
    instrumentLabel.replace(".csv","");
    return instrumentLabel;
}


const PeriodValSegmentPtr &InstrumentSelectionInfo::chartData()
{
    assert(chartData_);
    return chartData_;
}


void InstrumentSelectionInfo::setPatternMatches(const PatternMatchListPtr &patternMatches)
{

     patternMatches_ = patternMatches;

}

const PatternMatchListPtr &InstrumentSelectionInfo::patternMatches()
{
    assert(patternScanComplete()); // should not be called unless pattern scanning is complete
    return patternMatches_;
}

bool InstrumentSelectionInfo::patternScanComplete() const
{
    if(patternMatches_)
    {
        return true;
    }
    else
    {
        return false;
    }
}
