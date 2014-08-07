#ifndef DOWNTRIANGLEPLOTMARKER_H
#define DOWNTRIANGLEPLOTMARKER_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <qwt_plot_marker.h>

class DownTrianglePlotMarker : public QwtPlotMarker
{
public:
     DownTrianglePlotMarker(const std::string &title,
              const boost::posix_time::ptime &markerTime,
              double markerYVal);
};

#endif // DOWNTRIANGLEPLOTMARKER_H
