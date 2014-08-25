#include "StockChartDateScaleDraw.h"
#include <QDebug>
#include <math.h>
#include "QDateHelper.h"

StockChartDateScaleDraw::StockChartDateScaleDraw(Qt::TimeSpec /*timeSpec*/,
                                                    const PeriodValSegmentPtr &chartData)
    : chartData_(chartData)
{
    setDateFormat( QwtDate::Millisecond, "hh:mm:ss:zzz\nddd dd MMM" );
    setDateFormat( QwtDate::Second, "hh:mm:ss\nddd dd MMM" );
    setDateFormat( QwtDate::Minute, "hh:mm\nddd dd MMM" );
    setDateFormat( QwtDate::Hour, "hh:mm\nddd dd MMM" );
    setDateFormat( QwtDate::Day, "ddd dd MMM" );
    setDateFormat( QwtDate::Week, "Www" );
    setDateFormat( QwtDate::Month, "MMM" );
}

QwtText StockChartDateScaleDraw::label( double labelVal ) const
{
    qDebug() << "StockChartDateScaleDraw: label val=" << labelVal;

    unsigned int chartDataIndex = floor(labelVal);
    if(chartDataIndex < chartData_->numVals())
    {
        PeriodValCltn::iterator indexIter = chartData_->segBegin();
        std::advance(indexIter,chartDataIndex);
        QDateTime labelTime = QDateHelper::boostToQDateTime((*indexIter).periodTime());
        return QwtText(labelTime.toString(QString("MM/dd/yy")));
    }

    return QwtText(QString(""));

}
