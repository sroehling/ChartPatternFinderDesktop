#ifndef INSTRUMENTSELECTIONINFO_H
#define INSTRUMENTSELECTIONINFO_H

#include "PeriodValSegment.h"
#include <QString>
#include <boost/shared_ptr.hpp>


class InstrumentSelectionInfo
{
private:
    QString instrumentName_;
    PeriodValSegmentPtr chartData_;

public:
    InstrumentSelectionInfo(const QString &instrumentName,
                            const PeriodValSegmentPtr &chartData);

    const QString &instrumentName() const { return instrumentName_; }
    const PeriodValSegmentPtr &chartData() const { return chartData_; }
};

typedef boost::shared_ptr<InstrumentSelectionInfo> InstrumentSelectionInfoPtr;


#endif // INSTRUMENTSELECTIONINFO_H
