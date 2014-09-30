#include "VolumePlotCurve.h"
#include <qwt_date.h>
#include "QDateHelper.h"
#include "PseudoTimeOHLCSample.h"


VolumePlotCurve::VolumePlotCurve(const PeriodValSegmentPtr &chartData)
{

    QVector<QwtOHLCSample> chartDataSamples;
    chartDataSamples.reserve(chartData->numVals());
    for(PeriodValCltn::iterator chartDataIter = chartData->segBegin();
        chartDataIter != chartData->segEnd(); chartDataIter++)
    {
        double volume = (*chartDataIter).volume();

        // Overload the OHLC data to display the volume. This ensures when the volume
        // data will align with the candlesticks. Color-coding negative vs. positive volume is
        // also possible.
        double open = 0.0;
        double low = 0.0;
        double high = volume;
        double close = volume;

        chartDataSamples += PseudoTimeOHLCSample(
                    (*chartDataIter).pseudoXVal(),
                QwtDate::toDouble( QDateHelper::boostToQDateTime((*chartDataIter).periodTime())),
                    open,high,low,close);

    }
    setSamples( chartDataSamples );
    setTitle( "Volume" );
    setOrientation( Qt::Vertical );

    // The stock chart data is mapped onto an integral scale, with the
    // first value being 0, the second value 1, and so on. This allows
    // values to be aligned across weekends, etc. We therefore size
    // the bars for this integral scale.
    setSymbolExtent( 0.5 );
    setMinSymbolWidth( 3 );
    setMaxSymbolWidth( 12 );

    setSymbolPen( Qt::black );
    setSymbolBrush( QwtPlotTradingCurve::Decreasing, Qt::red );
    setSymbolBrush( QwtPlotTradingCurve::Increasing, Qt::white );


}
