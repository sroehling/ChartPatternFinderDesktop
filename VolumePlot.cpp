#include "VolumePlot.h"
#include "qwt_plot_barchart.h"
#include <qwt_date_scale_draw.h>
#include "StockChartDateScaleDraw.h"

#include <qwt_scale_engine.h>
#include <qwt_legend.h>
#include "qwt_plot_grid.h"

#include "VolumePlotCurve.h"
#include "VolumeYAxisScaleDraw.h"
#include <qwt_plot_legenditem.h>
#include <QDebug>
#include "DoubleRange.h"

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


    // Attach a legend internal to the plot
    QwtPlotLegendItem *legend = new QwtPlotLegendItem();
    legend->setAlignment(Qt::Alignment(Qt::AlignTop | Qt::AlignLeft));
    legend->attach(this);

    // Attach a dotted-line grid to the plot.
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setItemAttribute(grid->Legend, false);
    grid->setPen(QColor(Qt::gray), 0.0, Qt::PenStyle::DotLine);
    grid->attach(this);

    setAxisScaleDraw( QwtPlot::yLeft, new VolumeYAxisScaleDraw());
}

void VolumePlot::populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo)
{

    currentChartData_ = instrSelInfo;

    this->detachItems(QwtPlotItem::Rtti_PlotBarChart,true);
    this->detachItems(QwtPlotItem::Rtti_PlotTradingCurve,true);

    QwtDateScaleDraw *scaleDraw = new StockChartDateScaleDraw( Qt::UTC,instrSelInfo->chartData() );
    setAxisScaleDraw( QwtPlot::xBottom, scaleDraw );

    VolumePlotCurve *volDataCurve = new VolumePlotCurve(instrSelInfo->chartData());
    volDataCurve->attach( this );

    // Initally rescale the plot based upon the boundaries of the current chart data
    setAxisAutoScale( QwtPlot::yLeft );
    setAxisAutoScale( QwtPlot::xBottom);


    replot();

}

void VolumePlot::rescaleAxis(const QwtScaleDiv &xAxisScale)
{

    setAxisScaleDiv( QwtPlot::xBottom, xAxisScale );

    assert(currentChartData_);

    // Based upon the currently visible data values on the X axis, scale the Y axis for the
    // volume plot to what is visibile.

    double xAxisLowerBound = axisScaleDiv(QwtPlot::xBottom ).lowerBound();
    double xAxisUpperBound = axisScaleDiv(QwtPlot::xBottom ).upperBound();
    DoubleRange xAxisRange(xAxisLowerBound,xAxisUpperBound);
    qDebug() << "New volume plot x axis lower bound: " << xAxisUpperBound;
    qDebug() << "New volume plot x axis upper bound: " << xAxisLowerBound;

    double maxVolume = 0.0;
    PeriodValSegmentPtr chartData = currentChartData_->chartData();
    for(PeriodValCltn::iterator chartDataIter = chartData->segBegin();
        chartDataIter != chartData->segEnd(); chartDataIter++)
    {
        if(xAxisRange.valueWithinRange((*chartDataIter).pseudoXVal()))
        {
            double currVol = (*chartDataIter).volume();
            maxVolume = currVol>maxVolume?currVol:maxVolume;
        }

    }

    double roundUpFactor = 1000000.0;
    unsigned int fullVolMillions = (unsigned int)(maxVolume / roundUpFactor);
    double volumeMaxScale = ((double)fullVolMillions + 1.0)*roundUpFactor;

    setAxisScale(QwtPlot::yLeft,0,volumeMaxScale,0);


    replot();

}

