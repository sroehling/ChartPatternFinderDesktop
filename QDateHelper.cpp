#include "QDateHelper.h"

namespace QDateHelper
{

QDateTime boostToQDateTime(const boost::posix_time::ptime &boostTime)
{
    using namespace boost::posix_time;
    ptime epoch(boost::gregorian::date(1970,1,1));
    time_duration::sec_type secsSince1970Dur = (boostTime - epoch).total_seconds();
    // TODO - ... check overflow here ...
    time_t secsSince1970 = time_t(secsSince1970Dur);

    QDateTime convertedDateTime = QDateTime::fromTime_t(secsSince1970,Qt::UTC);

    return convertedDateTime;

}

}
