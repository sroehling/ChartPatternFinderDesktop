
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
#include <assert.h>

#include "plot.h"
#include "legend.h"
#include "griditem.h"

#include "StockChartDateScaleDraw.h"
#include "StockChartPlotZoomer.h"
#include "StockChartPlotCurve.h"
#include "BreakoutPlotMarker.h"
#include "PatternPlotCurve.h"
#include "QDateHelper.h"

#include "PeriodValSegment.h"
#include "DoubleBottomScanner.h"
#include "PatternShapeGenerator.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "SymetricWedgeScanner.h"
#include <sstream>
#include <qwt_scale_engine.h>

Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{
    setTitle( "Trading Chart" );

    QwtLinearScaleEngine *scaleEngine = new QwtLinearScaleEngine(10);

    setAxisTitle( QwtPlot::xBottom, QString( "Time" ) );
    setAxisScaleEngine( QwtPlot::xBottom, scaleEngine );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    setAxisTitle( QwtPlot::yLeft, QString( "Price" ) );

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    // TODO: Check the memory ownership/leak for the following allocation
    new StockChartPlotZoomer( canvas() );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );
}


void Plot::clearPatternPlots()
{
    // Detach and delete any existing plot curves
    this->detachItems(QwtPlotItem::Rtti_PlotCurve,true);
    this->detachItems(QwtPlotItem::Rtti_PlotMarker,true);

}

void Plot::populatePatternShapes(const PatternMatchPtr &patternMatch)
{
    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*patternMatch);
    PatternShapePointVectorVectorPtr curveShapes = patternShape->curveShapes();

    clearPatternPlots();

    // Re-populate with the pattern for the given patternMatch
    for(PatternShapePointVectorVector::iterator curveShapeIter = curveShapes->begin();
        curveShapeIter != curveShapes->end(); curveShapeIter++)
    {
        bool doCurveFit = true;
        QwtPlotCurve *patternMatchPlot = new PatternPlotCurve(*curveShapeIter,doCurveFit);
        patternMatchPlot->attach(this);
    }

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();
    for(PatternShapePointVectorVector::iterator lineShapeIter = lineShapes->begin();
        lineShapeIter != lineShapes->end(); lineShapeIter++)
    {
        bool doCurveFit = false;
        QwtPlotCurve *patternMatchPlot = new PatternPlotCurve(*lineShapeIter,doCurveFit);
        patternMatchPlot->attach(this);
    }


    if(patternMatch->breakoutInfo)
    {
        BreakoutPlotMarker *breakoutPlotMarker = new BreakoutPlotMarker(
                    patternMatch->breakoutInfo->pseudoXVal(),patternMatch->breakoutInfo->breakoutPrice());
        breakoutPlotMarker->attach(this);
    }

    replot();
    // TODO - Adjust the plot's visible area to encompass the pattern.

}



void Plot::populateChartData(const PeriodValSegmentPtr &chartData)
{
    GridItem *gridItem = new GridItem();
    gridItem->attach( this );

    clearPatternPlots();
    this->detachItems(QwtPlotItem::Rtti_PlotTradingCurve,true);


    QwtDateScaleDraw *scaleDraw = new StockChartDateScaleDraw( Qt::UTC,chartData );
    setAxisScaleDraw( QwtPlot::xBottom, scaleDraw );

    StockChartPlotCurve *chartDataCurve = new StockChartPlotCurve(chartData);
    chartDataCurve->attach( this );
    showItem( chartDataCurve, true );
    replot();

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
