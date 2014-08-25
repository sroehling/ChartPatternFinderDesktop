#include "StockChartPlotCurve.h"
#include <qwt_date.h>
#include "QDateHelper.h"
#include "PseudoTimeOHLCSample.h"


StockChartPlotCurve::StockChartPlotCurve(const PeriodValSegmentPtr &chartData)
{

    QVector<QwtOHLCSample> chartDataSamples;
    chartDataSamples.reserve(chartData->numVals());
    for(PeriodValCltn::iterator chartDataIter = chartData->segBegin();
        chartDataIter != chartData->segEnd(); chartDataIter++)
    {
        chartDataSamples += PseudoTimeOHLCSample(
                    (*chartDataIter).pseudoXVal(),
                QwtDate::toDouble( QDateHelper::boostToQDateTime((*chartDataIter).periodTime())),
                (*chartDataIter).open(), (*chartDataIter).high(), (*chartDataIter).low(), (*chartDataIter).close() );

    }
    setSamples( chartDataSamples );
    setTitle( "Symbol Name TBD" );
    setOrientation( Qt::Vertical );

    // The stock chart data is mapped onto an integral scale, with the
    // first value being 0, the second value 1, and so on. This allows
    // values to be aligned across weekends, etc. We therefore size
    // the bars for this integral scale.
    setSymbolExtent( 0.8 );
    setMinSymbolWidth( 3 );
    setMaxSymbolWidth( 15 );

    setSymbolPen( Qt::black );
    setSymbolBrush( QwtPlotTradingCurve::Decreasing, Qt::red );
    setSymbolBrush( QwtPlotTradingCurve::Increasing, Qt::white );


}
