#ifndef SCANPATTERNMATCHES_H
#define SCANPATTERNMATCHES_H

#include "InstrumentListTask.h"
#include "InstrumentSelectionInfo.h"

class ScanPatternMatches : public InstrumentListTask
{
private:
    InstrumentSelectionInfoPtr instrNeedingScan_;
public:
    ScanPatternMatches(const InstrumentSelectionInfoPtr &instrNeedingScan);
    virtual void doTask(const InstrumentListPtr &instrList);
};

#endif // SCANPATTERNMATCHES_H
