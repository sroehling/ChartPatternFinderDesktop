#include "ScanPatternMatches.h"

ScanPatternMatches::ScanPatternMatches(const InstrumentSelectionInfoPtr &instrNeedingScan)
    : instrNeedingScan_(instrNeedingScan)
{
}


void ScanPatternMatches::doTask(const InstrumentListPtr &/*instrList*/)
{
    instrNeedingScan_->scanPatternMatches();
}
