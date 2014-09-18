#include "StockChartPlotZoomer.h"
#include <qwt_date.h>
#include "QDateHelper.h"

#define STOCK_CHART_PLOT_ZOOMER_CENTER_ALIGN_X_ADJUSTMENT_FACTOR 0.5
#define STOCK_CHART_PLOT_ZOOMER_TRACKER_DATE_FORMAT "dd MMM yyyy"

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


void StockChartPlotZoomer::setChartData(const PeriodValSegmentPtr &chartData)
{
    chartData_ = chartData;
}

QwtText StockChartPlotZoomer::trackerTextF( const QPointF &pos ) const
{
    QwtText trackerText( "" );
    trackerText.setColor( Qt::white );
    QColor c = rubberBandPen().color();
    trackerText.setBorderPen( QPen( c ) );
    trackerText.setBorderRadius( 6 );
    c.setAlpha( 170 );
    trackerText.setBackgroundBrush( c );

    if(chartData_)
    {
        // On the chart, if pos.x() is only used, the date won't change until after
        // the candle. By adjusting the coordinates by 0.5, this aligns the date in
        // the middle.
        double centerAlignedTrackerXVal = pos.x() +
                STOCK_CHART_PLOT_ZOOMER_CENTER_ALIGN_X_ADJUSTMENT_FACTOR;

        unsigned int chartDataIndex = floor(centerAlignedTrackerXVal);
        if(chartDataIndex < chartData_->numVals())
        {
            PeriodValCltn::iterator indexIter = chartData_->segBegin();
            std::advance(indexIter,chartDataIndex);
            QDateTime labelTime = QDateHelper::boostToQDateTime((*indexIter).periodTime());
            trackerText.setText(labelTime.toString(QString(STOCK_CHART_PLOT_ZOOMER_TRACKER_DATE_FORMAT)));
        }
    }

    return trackerText;
}
