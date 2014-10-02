#include "BreakoutPlotMarker.h"
#include "QDateHelper.h"
#include <qwt_symbol.h>

BreakoutPlotMarker::BreakoutPlotMarker(
        double pseudoXVal,
        double markerYVal)
    :   QwtPlotMarker()
{


    setLineStyle( QwtPlotMarker::HLine );
    setLinePen( Qt::black, 1.25, Qt::DashLine );
    setVisible( true );

    QwtSymbol *markerSymbol = new QwtSymbol( QwtSymbol::UTriangle,
        QBrush( Qt::green ), QPen( Qt::black, 1 ), QSize( 12, 12 ) );

    setSymbol( markerSymbol );
    setValue( pseudoXVal,markerYVal);

}
