#ifndef STOCKCHARTPLOTZOOMER_H
#define STOCKCHARTPLOTZOOMER_H

#include <qwt_plot_zoomer.h>

class StockChartPlotZoomer: public QwtPlotZoomer
{
public:
    StockChartPlotZoomer( QWidget *canvas );

protected:
    virtual QwtText trackerTextF( const QPointF &pos ) const;
 };


#endif // STOCKCHARTPLOTZOOMER_H
