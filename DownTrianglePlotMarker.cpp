#include "DownTrianglePlotMarker.h"
#include "QDateHelper.h"
#include <qwt_symbol.h>

DownTrianglePlotMarker::DownTrianglePlotMarker(
        const std::string &title,
        const boost::posix_time::ptime &markerTime,
        double markerYVal)
    :   QwtPlotMarker()
{

    setTitle( QString( title.c_str() ) );
 //   highMarker->setLineStyle( QwtPlotMarker::HLine );
    setLinePen( Qt::black, 2, Qt::DashLine );
    setVisible( true );

    QwtSymbol *markerSymbol = new QwtSymbol( QwtSymbol::DTriangle,
        QBrush( Qt::yellow ), QPen( Qt::red, 1 ), QSize( 10, 10 ) );

    setSymbol( markerSymbol );
    setValue( QwtDate::toDouble( QDateHelper::boostToQDateTime(markerTime) ),
                          markerYVal);

}
