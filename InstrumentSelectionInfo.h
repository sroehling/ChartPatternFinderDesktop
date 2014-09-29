#ifndef INSTRUMENTSELECTIONINFO_H
#define INSTRUMENTSELECTIONINFO_H

#include "PeriodValSegment.h"
#include <QString>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <QDir>
#include "PatternMatch.h"


class InstrumentSelectionInfo
{
private:
    QDir dir_;
    QString fileName_;

    PeriodValSegmentPtr chartData_;
    PatternMatchListPtr patternMatches_;

public:
    InstrumentSelectionInfo(const QDir &directory, const QString &fileName);

    QString instrumentName() const;
    const PeriodValSegmentPtr &chartData();

    void scanPatternMatches();
    const PatternMatchListPtr &patternMatches();

    bool patternScanComplete() const;

    // Reserved for InstrumentList. When a thread starts scanning, it will
    // mark this as true, preventing other threads from scanning at the same time.
    bool patternScanInProgress;
};

typedef boost::shared_ptr<InstrumentSelectionInfo> InstrumentSelectionInfoPtr;
typedef std::vector<InstrumentSelectionInfoPtr> InstrumentSelectionInfoVector;


#endif // INSTRUMENTSELECTIONINFO_H
