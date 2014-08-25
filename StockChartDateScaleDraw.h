#ifndef STOCKCHARTDATESCALEDDRAW_H
#define STOCKCHARTDATESCALEDDRAW_H

#include <qwt_date_scale_draw.h>

#include "PeriodValSegment.h"

class StockChartDateScaleDraw : public QwtDateScaleDraw
{
private:
    PeriodValSegmentPtr chartData_;
public:
    StockChartDateScaleDraw( Qt::TimeSpec timeSpec, const PeriodValSegmentPtr &chartData );
    virtual QwtText label( double ) const;
};

#endif // STOCKCHARTDATESCALEDDRAW_H
