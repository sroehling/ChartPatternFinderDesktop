#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "StockChartPlotZoomer.h"
#include "InstrumentSelectionInfo.h"

class Plot: public QwtPlot
{
    Q_OBJECT

private:
    StockChartPlotZoomer *plotZoomer_;


public:
    Plot( QWidget * = NULL );
    void populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo);

    void clearPatternPlots();
    void populateOnePatternShape(const PatternMatchPtr &patternMatch);
    void populatePatternMatchesShapes(const PatternMatchListPtr &patternMatches);

public Q_SLOTS:
    void setMode( int );
    void exportPlot();

private Q_SLOTS:
    void showItem( QwtPlotItem *, bool on );
};

#endif
