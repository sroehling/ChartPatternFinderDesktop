#ifndef PATTERNPLOTCURVE_H
#define PATTERNPLOTCURVE_H

#include <qwt_plot_curve.h>
#include "PatternShapePoint.h"

class PatternPlotCurve : public QwtPlotCurve
{
public:
    PatternPlotCurve(const PatternShapePointVectorPtr &shapeCurve);
};

#endif // PATTERNPLOTCURVE_H
