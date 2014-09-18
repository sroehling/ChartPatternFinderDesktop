#include "InstrumentSelectionInfo.h"

InstrumentSelectionInfo::InstrumentSelectionInfo(const QString &instrumentName,
                           const PeriodValSegmentPtr &chartData)
    :instrumentName_(instrumentName),
      chartData_(chartData)
{
}
