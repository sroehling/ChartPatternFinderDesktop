#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "StockChartPlotZoomer.h"

class Plot: public QwtPlot
{
    Q_OBJECT


private:
    StockChartPlotZoomer *plotZoomer_;

public:
    Plot( QWidget * = NULL );
    void populateChartData(const PeriodValSegmentPtr &chartData);
    void populatePatternShapes(const PatternMatchPtr &patternMatch);

public Q_SLOTS:
    void setMode( int );
    void exportPlot();

private Q_SLOTS:
    void showItem( QwtPlotItem *, bool on );
    void clearPatternPlots();
};

#endif
