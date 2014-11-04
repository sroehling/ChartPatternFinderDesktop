#include "InstrumentListWorker.h"
#include "InstrumentListTask.h"
#include "InstrumentList.h"

InstrumentListWorker::InstrumentListWorker(const InstrumentListPtr &instrList)
    : instrList_(instrList)
{
}

void InstrumentListWorker::run()
{
    InstrumentListTaskPtr instrListTask = instrList_->nextInstrListTask();
    while(instrListTask)
    {
        instrListTask->doTask(instrList_);
        instrListTask = instrList_->nextInstrListTask();
    }

}
