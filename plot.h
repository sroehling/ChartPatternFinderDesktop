#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include "PatternMatch.h"
#include "PeriodValSegment.h"

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );

public Q_SLOTS:
    void setMode( int );
    void exportPlot();

private Q_SLOTS:
    void showItem( QwtPlotItem *, bool on );

private:
    void populate();
    void populatePatternShapes(const PatternMatchPtr &patternMatch);
    void populateDoubleBottomPatterns(const PeriodValSegmentPtr &chartData);
    void populateSymetricWedgePatterns(const PeriodValSegmentPtr &chartData);
};

#endif
