#include "PseudoTimeOHLCSample.h"

PseudoTimeOHLCSample::PseudoTimeOHLCSample(double pseudoTime, double realTime,
                                                                double open, double high,
                                                                double low, double close)
: QwtOHLCSample(pseudoTime,open,high,low,close),
           realTime_(realTime)
{
}
