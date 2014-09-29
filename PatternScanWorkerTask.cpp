#include "PatternScanWorkerTask.h"

PatternScanWorkerTask::PatternScanWorkerTask(const InstrumentListPtr &instrList)
: instrList_(instrList)
{
}


void PatternScanWorkerTask::run()
{
    InstrumentSelectionInfoPtr instrForPatternScan = instrList_->nextInstrumentForPatternScan();
    while(instrForPatternScan)
    {
        instrForPatternScan->scanPatternMatches();
        instrForPatternScan = instrList_->nextInstrumentForPatternScan();
    }
}
