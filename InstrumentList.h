#ifndef INSTRUMENTLIST_H
#define INSTRUMENTLIST_H

#include <boost/shared_ptr.hpp>
#include <QMutex>
#include "InstrumentSelectionInfo.h"

class InstrumentList
{
private:
    InstrumentSelectionInfoVector instrumentInfo_;

    QMutex instrumentInfoMutex_;

    // The list can be marked "obsolete", after which
    // nextInstrumentForPatternScan() will return a NULL
    // smart pointer. If the user switches to a new instrument
    // list, this allows the scanning threads to gracefully exit.
    bool listIsObsolete_;

public:
    InstrumentList(const QString &quoteFilePath);

    unsigned int numInstruments() const { return instrumentInfo_.size(); }

    // Return the instrument info, without confirming whether or not the
    // patterns have been scanned. This is useful for displaying the instrument
    // information even before the patterns are finished scanning.
    const InstrumentSelectionInfoPtr &instrInfo(unsigned int instrNum) const;

    const InstrumentSelectionInfoPtr instrInfoWithScannedPatterns(unsigned int instrNum);

    // Return the next instrument for scanning, or NULL if there are no more.
    InstrumentSelectionInfoPtr nextInstrumentForPatternScan();

    void obsoleteList();

};

typedef boost::shared_ptr<InstrumentList> InstrumentListPtr;

#endif // INSTRUMENTLIST_H
