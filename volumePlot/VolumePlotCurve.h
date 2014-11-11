#ifndef VOLUMEPLOTCURVE_H
#define VOLUMEPLOTCURVE_H

#include "PeriodValSegment.h"
#include "qwt_plot_tradingcurve.h"

class VolumePlotCurve : public QwtPlotTradingCurve
{
public:
    VolumePlotCurve(const PeriodValSegmentPtr &chartData);
};

#endif // VOLUMEPLOTCURVE_H
