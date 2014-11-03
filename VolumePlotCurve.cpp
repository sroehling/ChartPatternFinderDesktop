#include "VolumePlotCurve.h"
#include <qwt_date.h>
#include "QDateHelper.h"
#include "PseudoTimeOHLCSample.h"


VolumePlotCurve::VolumePlotCurve(const PeriodValSegmentPtr &chartData)
{

    QVector<QwtOHLCSample> chartDataSamples;
    chartDataSamples.reserve(chartData->numVals());

    bool firstVol = true;
    double prevClose = -1.0;
    for(PeriodValCltn::iterator chartDataIter = chartData->segBegin();
        chartDataIter != chartData->segEnd(); chartDataIter++)
    {
        double volume = (*chartDataIter).volume();

        // Overload the open,high,low, close data to hold
        // the "starting" and "ending" volume. The height of the
        // volume bar is always the actual volume, but the opening
        // and closing values are toggled to show the closing price
        // rising or closing with the volume.
        double startVol = 0.0;
        double endVol = 0.0;
        if(firstVol)
        {
            if((*chartDataIter).close() > (*chartDataIter).open())
            {
                startVol = 0.0;
                endVol = volume;
            }
            else
            {
                startVol = volume;
                endVol = 0.0;
            }

            firstVol = false;
        }
        else
        {
            assert(prevClose >= 0.0);
            if((*chartDataIter).close() >= prevClose)
            {
                startVol = 0.0;
                endVol = volume;
            }
            else
            {
                startVol = volume;
                endVol = 0.0;
            }
        }
        prevClose = (*chartDataIter).close();

        double open, high, low, close;

        if(startVol < endVol)
        {
            open = low = startVol;
            high = close = endVol;
        }
        else
        {
            open = high = startVol;
            low = close = endVol;
        }

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
    setSymbolBrush( QwtPlotTradingCurve::Increasing, Qt::darkGray );


}
