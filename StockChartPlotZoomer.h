#ifndef STOCKCHARTPLOTZOOMER_H
#define STOCKCHARTPLOTZOOMER_H

#include <qwt_plot_zoomer.h>
#include "PeriodValSegment.h"

class StockChartPlotZoomer: public QwtPlotZoomer
{
private:
    PeriodValSegmentPtr chartData_;
public:
    StockChartPlotZoomer( QWidget *canvas );

    void setChartData(const PeriodValSegmentPtr &chartData);

protected:
    virtual QwtText trackerTextF( const QPointF &pos ) const;
 };


#endif // STOCKCHARTPLOTZOOMER_H
