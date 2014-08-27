#ifndef BREAKOUTPLOTMARKER_H
#define BREAKOUTPLOTMARKER_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <qwt_plot_marker.h>

class BreakoutPlotMarker : public QwtPlotMarker
{
public:
     BreakoutPlotMarker(double pseudoXVal,
              double markerYVal);
};

#endif // BREAKOUTPLOTMARKER_H
