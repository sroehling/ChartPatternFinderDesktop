#ifndef PATTERNPLOTCURVE_H
#define PATTERNPLOTCURVE_H

#include <qwt_plot_curve.h>
#include "PatternMatch.h"

class PatternPlotCurve : public QwtPlotCurve
{
public:
    PatternPlotCurve(const PatternMatchPtr &patternMatch);
};

#endif // PATTERNPLOTCURVE_H
