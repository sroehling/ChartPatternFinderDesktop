#include "BreakoutPlotMarker.h"
#include "QDateHelper.h"
#include <qwt_symbol.h>

BreakoutPlotMarker::BreakoutPlotMarker(
        double pseudoXVal,
        double markerYVal)
    :   QwtPlotMarker()
{

    setLineStyle( QwtPlotMarker::HLine );
    setLinePen( Qt::black, 1, Qt::DashLine );
    setVisible( true );

    QwtSymbol *markerSymbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::green ), QPen( Qt::black, 1 ), QSize( 10, 10 ) );

    setSymbol( markerSymbol );
    setValue( pseudoXVal,markerYVal);

}
