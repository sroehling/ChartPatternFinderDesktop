#ifndef INSTRUMENTLISTTYPES_H
#define INSTRUMENTLISTTYPES_H

#include <boost/shared_ptr.hpp>
#include <list>


class InstrumentList;
typedef boost::shared_ptr<InstrumentList> InstrumentListPtr;

class InstrumentListTask;
typedef boost::shared_ptr<InstrumentListTask> InstrumentListTaskPtr;
typedef std::list<InstrumentListTaskPtr> InstrumentListTaskList;


#endif // INSTRUMENTLISTTYPES_H
