#ifndef PATTERNSCANWORKERTASK_H
#define PATTERNSCANWORKERTASK_H

#include <QRunnable>
#include "InstrumentList.h"

class PatternScanWorkerTask : public QRunnable
{
private:
    InstrumentListPtr instrList_;
public:
    PatternScanWorkerTask(const InstrumentListPtr &instrList);

    void run();

};

#endif // PATTERNSCANWORKERTASK_H
