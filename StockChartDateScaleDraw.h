#ifndef STOCKCHARTDATESCALEDDRAW_H
#define STOCKCHARTDATESCALEDDRAW_H

#include <qwt_date_scale_draw.h>


class StockChartDateScaleDraw : public QwtDateScaleDraw
{
public:
    StockChartDateScaleDraw( Qt::TimeSpec timeSpec );
};

#endif // STOCKCHARTDATESCALEDDRAW_H
