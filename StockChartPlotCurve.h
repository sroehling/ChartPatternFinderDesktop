#ifndef STOCKCHARTPLOTCURVE_H
#define STOCKCHARTPLOTCURVE_H

#include <qwt_plot_tradingcurve.h>
#include "PeriodValSegment.h"

class StockChartPlotCurve : public QwtPlotTradingCurve
{
public:
    StockChartPlotCurve(const PeriodValSegmentPtr &chartData);
};

#endif // STOCKCHARTPLOTCURVE_H
