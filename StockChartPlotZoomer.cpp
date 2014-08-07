#include "StockChartPlotZoomer.h"
#include <qwt_date.h>

StockChartPlotZoomer::StockChartPlotZoomer( QWidget *canvas ):
    QwtPlotZoomer( canvas )
{
    setRubberBandPen( QColor( Qt::darkGreen ) );
    setTrackerMode( QwtPlotPicker::AlwaysOn );

    setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

}


QwtText StockChartPlotZoomer::trackerTextF( const QPointF &pos ) const
{
    const QDateTime dt = QwtDate::toDateTime( pos.x() );

    QString s;
    s += QwtDate::toString( QwtDate::toDateTime( pos.x() ),
        "MMM dd hh:mm ", QwtDate::FirstThursday );

    QwtText text( s );
    text.setColor( Qt::white );

    QColor c = rubberBandPen().color();
    text.setBorderPen( QPen( c ) );
    text.setBorderRadius( 6 );
    c.setAlpha( 170 );
    text.setBackgroundBrush( c );

    return text;
}
