#include "StockChartPlotCurve.h"
#include <qwt_date.h>
#include "QDateHelper.h"


StockChartPlotCurve::StockChartPlotCurve(const PeriodValSegmentPtr &chartData)
{

    QVector<QwtOHLCSample> chartDataSamples;
    chartDataSamples.reserve(chartData->numVals());
    for(PeriodValCltn::iterator chartDataIter = chartData->segBegin();
        chartDataIter != chartData->segEnd(); chartDataIter++)
    {
        std::cerr << "Chart data: " << (*chartDataIter) << std::endl;
        chartDataSamples += QwtOHLCSample(
                QwtDate::toDouble( QDateHelper::boostToQDateTime((*chartDataIter).periodTime())),
                (*chartDataIter).open(), (*chartDataIter).high(), (*chartDataIter).low(), (*chartDataIter).close() );

    }
  //  QwtPlotTradingCurve *chartDataCurve = new QwtPlotTradingCurve();
    setSamples( chartDataSamples );
    setTitle( "Symbol Name TBD" );
    setOrientation( Qt::Vertical );

    // as we have one sample per day a symbol width of
    // 12h avoids overlapping symbols. We also bound
    // the width, so that is is not scaled below 3 and
    // above 15 pixels.

    setSymbolExtent( 12 * 3600 * 1000.0 );
    setMinSymbolWidth( 3 );
    setMaxSymbolWidth( 15 );

    setSymbolPen( Qt::darkMagenta );
    setSymbolBrush( QwtPlotTradingCurve::Decreasing, Qt::darkMagenta );
    setSymbolBrush( QwtPlotTradingCurve::Increasing, Qt::white );


}
