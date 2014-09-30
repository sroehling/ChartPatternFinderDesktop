#ifndef STACKEDSTOCKCHARTS_H
#define STACKEDSTOCKCHARTS_H

#include <QFrame>
#include <qframe.h>
#include <qwt_plot.h>
#include "plot.h"
#include "VolumePlot.h"
#include <QGridLayout>

class StackedStockCharts : public QFrame
{
    Q_OBJECT


private:
    Plot *priceAndPatternPlot_;
    VolumePlot *volumePlot_;

public:
    StackedStockCharts(QWidget * parent = NULL );
    virtual ~StackedStockCharts() {}

    void populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo);
    Plot *priceAndPatternPlot() const { return priceAndPatternPlot_; }

private Q_SLOTS:
    void scaleDivChanged();

};

#endif // STACKEDSTOCKCHARTS_H
