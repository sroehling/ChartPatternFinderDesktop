#ifndef INSTRUMENTLISTTASK_H
#define INSTRUMENTLISTTASK_H

#include "InstrumentListTypes.h"

class InstrumentListTask
{
public:
    InstrumentListTask();
    virtual void doTask(const InstrumentListPtr &instrList) = 0;

    virtual ~InstrumentListTask() {}
};


#endif // INSTRUMENTLISTTASK_H
