#include "StackedStockCharts.h"
#include <QGridLayout>
#include <qlayout.h>
#include <qpen.h>
#include <qwt_plot.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include "plot.h"
#include <QDebug>

StackedStockCharts::StackedStockCharts(QWidget *parent ):
    QFrame( parent )
{

    QGridLayout *layout = new QGridLayout( this );

    priceAndPatternPlot_ = new Plot(this);
    layout->addWidget( priceAndPatternPlot_,0,0 );

    volumePlot_ = new VolumePlot(this);
    layout->addWidget( volumePlot_, 1, 0 );

    layout->setRowStretch(0,80);
    layout->setRowStretch(1,20);

    connect( priceAndPatternPlot_->axisWidget( QwtPlot::xBottom ),
        SIGNAL( scaleDivChanged() ), SLOT( scaleDivChanged() ) );

}

void StackedStockCharts::populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo)
{
    volumePlot_->populateChartData(instrSelInfo);
    priceAndPatternPlot_->populateChartData(instrSelInfo);

    QwtScaleWidget *priceScaleWidget = priceAndPatternPlot_->axisWidget( QwtPlot::yLeft );
    QwtScaleDraw *priceScaleDraw = priceScaleWidget->scaleDraw();
    priceScaleDraw->setMinimumExtent( 0.0 );
    double priceExtent = priceScaleDraw->extent( priceScaleWidget->font() );

    QwtScaleWidget *volScaleWidget = volumePlot_->axisWidget( QwtPlot::yLeft );
    QwtScaleDraw *volScaleDraw = volScaleWidget->scaleDraw();
    volScaleDraw->setMinimumExtent( 0.0 );
    double volExtent = volScaleDraw->extent( volScaleWidget->font() );

    double maxExtent = priceExtent>volExtent?priceExtent:volExtent;

    volScaleDraw->setMinimumExtent(maxExtent);
    priceScaleDraw->setMinimumExtent(maxExtent);

    scaleDivChanged();
}


void StackedStockCharts::scaleDivChanged()
{

    qDebug() << "Stacked Stock Charts: scaleDivChange()";

    volumePlot_->setAxisScaleDiv( QwtPlot::xBottom, priceAndPatternPlot_->axisScaleDiv( QwtPlot::xBottom ) );

    // TODO - Based upon the currently visible data values on the X axis, scale the Y axis for the
    // volume plot to what is visibile.
    // volumePlot_->setAxisAutoScale( QwtPlot::yLeft );


    volumePlot_->replot();

}
