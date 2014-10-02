#include "BreakdownPlotMarker.h"

#include "QDateHelper.h"
#include <qwt_symbol.h>

BreakdownPlotMarker::BreakdownPlotMarker(
        double pseudoXVal,
        double markerYVal)
    :   QwtPlotMarker()
{

    setLineStyle( QwtPlotMarker::HLine );
    setLinePen( Qt::black, 1.25, Qt::DashLine );
    setVisible( true );

    QwtSymbol *markerSymbol = new QwtSymbol( QwtSymbol::DTriangle,
        QBrush( Qt::green ), QPen( Qt::black, 1 ), QSize( 12, 12 ) );

    setSymbol( markerSymbol );
    setValue( pseudoXVal,markerYVal);

}
