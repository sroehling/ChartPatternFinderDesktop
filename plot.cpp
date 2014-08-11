
#include <qwt_legend.h>
#include <qwt_plot_zoneitem.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_panner.h>
#include <qwt_legend_label.h>
#include <qwt_date.h>
#include <qwt_date_scale_engine.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_plot_barchart.h>

#include "plot.h"
#include "legend.h"
#include "griditem.h"

#include "StockChartDateScaleDraw.h"
#include "StockChartPlotZoomer.h"
#include "StockChartPlotCurve.h"
#include "DownTrianglePlotMarker.h"
#include "PatternPlotCurve.h"
#include "QDateHelper.h"

#include "PeriodValSegment.h"
#include "DoubleBottomScanner.h"
#include "PatternShapeGenerator.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include <sstream>

Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{
    setTitle( "Trading Chart" );

    QwtDateScaleDraw *scaleDraw = new StockChartDateScaleDraw( Qt::UTC );
    QwtDateScaleEngine *scaleEngine = new QwtDateScaleEngine( Qt::UTC );

    setAxisTitle( QwtPlot::xBottom, QString( "Time" ) );
    setAxisScaleDraw( QwtPlot::xBottom, scaleDraw );
    setAxisScaleEngine( QwtPlot::xBottom, scaleEngine );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    setAxisTitle( QwtPlot::yLeft, QString( "Price" ) );

#if 0
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable );
    insertLegend( legend, QwtPlot::RightLegend );
#else
    Legend *legend = new Legend;
    insertLegend( legend, QwtPlot::RightLegend );
#endif

    populate();

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    // TODO: Check the memory ownership/leak for the following allocation
    new StockChartPlotZoomer( canvas() );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );

    connect( legend, SIGNAL( checked( QwtPlotItem *, bool, int ) ),
        SLOT( showItem( QwtPlotItem *, bool ) ) );
}


void Plot::populate()
{
    GridItem *gridItem = new GridItem();
#if 0
    gridItem->setOrientations( Qt::Horizontal );
#endif
    gridItem->attach( this );


 //   PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("/tmp/aapl.csv");
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile(
     "/Users/sroehling/Development/workspace/PatternRecognitionDesktop/lib/PatternRecognitionLib/test/patternShape/QCOR_2013_2014_Weekly.csv");
    StockChartPlotCurve *chartDataCurve = new StockChartPlotCurve(chartData);
    chartDataCurve->attach( this );
    showItem( chartDataCurve, true );

    DownTrianglePlotMarker *highMarker = new DownTrianglePlotMarker("High",
                    chartData->highestHighVal().periodTime(),
                    chartData->highestHighVal().high());
    highMarker->attach(this);

    // Overlay the first pattern match on the chart.
    PatternScannerPtr doubleBottomScanner(new DoubleBottomScanner(DoubleRange(7.0,40.0)));
    MultiPatternScanner multiScanner(doubleBottomScanner);
    PatternMatchListPtr doubleBottoms = multiScanner.scanUniquePatternMatches(chartData);
    assert(doubleBottoms->size() > 0);
    QwtPlotCurve *patternMatchPlot = new PatternPlotCurve(doubleBottoms->front());
    patternMatchPlot->attach(this);


 /*
  *
  * TODO - Move the example code below to separate classes.

    // Plot some dummy bar chart data for volume. This is
    QVector<QPointF> volumeData;
    for ( int i = 0; i < 100; i+=2 )
    {
        double barXVal = QwtDate::toDouble( year2010.addDays( (i+1) ));
        double dummiedUpYVal = 50 + i * 0.25;
        volumeData += QPointF(barXVal,dummiedUpYVal);
    }

    QwtPlotBarChart *volumeBars = new QwtPlotBarChart();
    volumeBars->setTitle( "Volume" );
    volumeBars->setSpacing(4);

    volumeBars->setSamples(volumeData);

    volumeBars->attach(this);
*/

}

void Plot::setMode( int style )
{
    QwtPlotTradingCurve::SymbolStyle symbolStyle =
        static_cast<QwtPlotTradingCurve::SymbolStyle>( style );

    QwtPlotItemList curves = itemList( QwtPlotItem::Rtti_PlotTradingCurve );
    for ( int i = 0; i < curves.size(); i++ )
    {
        QwtPlotTradingCurve *curve =
            static_cast<QwtPlotTradingCurve *>( curves[i] );
        curve->setSymbolStyle( symbolStyle );
    }

    replot();
}

void Plot::showItem( QwtPlotItem *item, bool on )
{
    item->setVisible( on );
    replot();
}

void Plot::exportPlot()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( this, "stockchart.pdf" );
}
