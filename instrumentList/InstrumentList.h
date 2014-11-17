#ifndef INSTRUMENTLIST_H
#define INSTRUMENTLIST_H

#include <boost/shared_ptr.hpp>
#include <QMutex>
#include <QObject>
#include "InstrumentSelectionInfo.h"
#include "InstrumentListTypes.h"

class InstrumentList: public QObject
{
        Q_OBJECT
private:
    InstrumentSelectionInfoVector instrumentInfo_;

    QMutex instrumentInfoMutex_;

    // The list can be marked "obsolete", after which
    // nextInstrumentForPatternScan() will return a NULL
    // smart pointer. If the user switches to a new instrument
    // list, this allows the scanning threads to gracefully exit.
    bool listIsObsolete_;

    // Number of CSV files scanned for quotes
    unsigned int numCSVFiles_;
    unsigned int numInstrsScanned_;

    InstrumentListTaskList taskList_;

public:
    InstrumentList(const QString &quoteFilePath);

    unsigned int numInstruments() const { return instrumentInfo_.size(); }

    // Return the instrument info, without confirming whether or not the
    // patterns have been scanned. This is useful for displaying the instrument
    // information even before the patterns are finished scanning.
    const InstrumentSelectionInfoPtr &instrInfo(unsigned int instrNum) const;

    void addInstrumentSelectionInfo(const InstrumentSelectionInfoPtr &instrSelInfo);

    void oneInstrScanComplete();

    const InstrumentSelectionInfoPtr instrInfoWithScannedPatterns(unsigned int instrNum);

    // Return the next instrument list task (reading or scanning), or NULL if there are no more.
    InstrumentListTaskPtr nextInstrListTask();

    void obsoleteList();
    bool listIsObsolete() const { return listIsObsolete_; }

    unsigned int numCSVFiles() const { return numCSVFiles_; }

signals:
    void instrumentAdded(unsigned int instrNum);
    void instrumentScanProgressUpdated(unsigned int numInstrsScanned);
    void instrScanComplete();

};


#endif // INSTRUMENTLIST_H
