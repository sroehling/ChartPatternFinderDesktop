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
    const PatternMatchListPtr &patternMatches();
};

typedef boost::shared_ptr<InstrumentSelectionInfo> InstrumentSelectionInfoPtr;
typedef std::vector<InstrumentSelectionInfoPtr> InstrumentSelectionInfoVector;


#endif // INSTRUMENTSELECTIONINFO_H
