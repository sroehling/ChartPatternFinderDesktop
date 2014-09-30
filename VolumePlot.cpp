#include "VolumePlot.h"
#include "qwt_plot_barchart.h"
#include <qwt_date_scale_draw.h>
#include "StockChartDateScaleDraw.h"

#include <qwt_scale_engine.h>
#include <qwt_legend.h>

#include "VolumePlotCurve.h"

VolumePlot::VolumePlot(QWidget *parent) :
    QwtPlot(parent)
{
    QwtLinearScaleEngine *scaleEngine = new QwtLinearScaleEngine(10);

    setAxisScaleEngine( QwtPlot::xBottom, scaleEngine );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );

    // The following is needed to properly adjust the RHS of the X axis. Otherwise,
    // there is space on the RHS.
    axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating,true);

    // Set the background color to white
    setAutoFillBackground( true );
    setPalette( Qt::white );
    setCanvasBackground( Qt::white );

    // Override the size policy. Otherwise, the plot may not scale to
    // the desired dimensions from the grid layout.
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMinimumSize(0,0);

    QwtLegend *legend = new QwtLegend;
    this->insertLegend(legend, QwtPlot::BottomLegend);

}

void VolumePlot::populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo)
{

    this->detachItems(QwtPlotItem::Rtti_PlotBarChart,true);
    this->detachItems(QwtPlotItem::Rtti_PlotTradingCurve,true);

    QwtDateScaleDraw *scaleDraw = new StockChartDateScaleDraw( Qt::UTC,instrSelInfo->chartData() );
    setAxisScaleDraw( QwtPlot::xBottom, scaleDraw );

    VolumePlotCurve *volDataCurve = new VolumePlotCurve(instrSelInfo->chartData());
    volDataCurve->attach( this );

    // Rescale the plot based upon the boundaries of the current chart data
    setAxisAutoScale( QwtPlot::yLeft );
    setAxisAutoScale( QwtPlot::xBottom);


    replot();


}

