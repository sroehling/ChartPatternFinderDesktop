#ifndef VOLUMEPLOT_H
#define VOLUMEPLOT_H

#include <qwt_plot.h>
#include "InstrumentSelectionInfo.h"

class VolumePlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit VolumePlot(QWidget *parent = 0);

    void populateChartData(const InstrumentSelectionInfoPtr &instrSelInfo);


signals:

public slots:

};

#endif // VOLUMEPLOT_H
