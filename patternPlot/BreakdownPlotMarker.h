#ifndef BREAKDOWNPLOTMARKER_H
#define BREAKDOWNPLOTMARKER_H

#include <qwt_plot_marker.h>

class BreakdownPlotMarker : public QwtPlotMarker
{
public:
     BreakdownPlotMarker(double pseudoXVal,
              double markerYVal);
};

#endif // BREAKDOWNPLOTMARKER_H
