#ifndef VOLUMEPLOT_H
#define VOLUMEPLOT_H

#include <qwt_plot.h>
#include "InstrumentSelectionInfo.h"

class VolumePlot : public QwtPlot
{
    Q_OBJECT

private:
    InstrumentSelectionInfoPtr currentChartData_;

public:
    explicit VolumePlot(QWidget *parent = 0);

    void populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo);

    // The X axis for volume chart needs to stay in sync with the price chart.
    // rescaleAxis() will adjust the x axis for the volume plot to match
    // the given scale. It will then adjust the y axis to have a maximum
    // value just above the maximum volume value in currentChartData_.
    void rescaleAxis(const QwtScaleDiv &xAxisScale);
signals:

public slots:

};

#endif // VOLUMEPLOT_H
