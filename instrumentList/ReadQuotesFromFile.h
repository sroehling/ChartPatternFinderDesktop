#ifndef READQUOTESFROMFILE_H
#define READQUOTESFROMFILE_H

#include <QDir>
#include <QString>
#include "InstrumentListTask.h"

class ReadQuotesFromFile : public InstrumentListTask
{
private:
    QDir dir_;
    QString fileName_;
public:
    ReadQuotesFromFile(const QDir &directory, const QString &fileName);

    virtual void doTask(InstrumentList &instrList);
};

#endif // READQUOTESFROMFILE_H
