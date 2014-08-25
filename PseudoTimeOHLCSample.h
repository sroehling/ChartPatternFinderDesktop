#ifndef PSEUDOTIMEOHLCSAMPLE_H
#define PSEUDOTIMEOHLCSAMPLE_H

#include <qwt_samples.h>


// To eliminate gaps in the chart for weekends, holidays, etc.
// a "pseudoTime" is used instead of the real time. This pseudoTime
// is just an index into the period values for the chart data.
// This same index is used by StockChartDateScaleDraw to do a lookup
// of real timess based upon pseudoTime.
class PseudoTimeOHLCSample : public QwtOHLCSample
{
private:
    double realTime_;
public:
    PseudoTimeOHLCSample(double pseudoTime, double realTime,
                         double open, double high,
                         double low, double close);
};

#endif // PSEUDOTIMEOHLCSAMPLE_H
