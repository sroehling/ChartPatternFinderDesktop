#ifndef INSTRUMENTLISTWORKER_H
#define INSTRUMENTLISTWORKER_H

#include <QRunnable>
#include "InstrumentListTypes.h"

class InstrumentListWorker : public QRunnable
{
private:
    InstrumentListPtr instrList_;
public:
    InstrumentListWorker(const InstrumentListPtr &instrList);

    void run();
};

#endif // INSTRUMENTLISTWORKER_H
