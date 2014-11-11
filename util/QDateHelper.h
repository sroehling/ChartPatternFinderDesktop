#ifndef QDATEHELPER_H
#define QDATEHELPER_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <qwt_date.h>


namespace QDateHelper
{
    QDateTime boostToQDateTime(const boost::posix_time::ptime &boostTime);
}

#endif // QDATEHELPER_H
