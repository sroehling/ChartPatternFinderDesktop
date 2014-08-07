#include "StockChartDateScaleDraw.h"

StockChartDateScaleDraw::StockChartDateScaleDraw(Qt::TimeSpec /*timeSpec*/ )
{
    setDateFormat( QwtDate::Millisecond, "hh:mm:ss:zzz\nddd dd MMM" );
    setDateFormat( QwtDate::Second, "hh:mm:ss\nddd dd MMM" );
    setDateFormat( QwtDate::Minute, "hh:mm\nddd dd MMM" );
    setDateFormat( QwtDate::Hour, "hh:mm\nddd dd MMM" );
    setDateFormat( QwtDate::Day, "ddd dd MMM" );
    setDateFormat( QwtDate::Week, "Www" );
    setDateFormat( QwtDate::Month, "MMM" );
}
