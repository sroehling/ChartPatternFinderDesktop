#include "ScanPatternMatches.h"
#include "DescendingTriangleScanner.h"
#include "AscendingTriangleScanner.h"
#include "CupWithHandleScanner.h"
#include "CupWithoutHandleScanner.h"
#include "RectangleScanner.h"
#include "PivotHighScanner.h"
#include "RisingWedgeScanner.h"
#include "FallingWedgeScanner.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "SymetricTriangleScanner.h"
#include "DoubleBottomScanner.h"
#include "PeriodValSegment.h"
#include "PatternMatch.h"
#include "InstrumentList.h"

ScanPatternMatches::ScanPatternMatches(const InstrumentSelectionInfoPtr &instrNeedingScan)
    : instrNeedingScan_(instrNeedingScan)
{
}


void ScanPatternMatches::doTask(InstrumentList &instrList)
{

    PatternMatchListPtr currentPatternMatches = PatternMatchListPtr(new PatternMatchList());
    PeriodValSegmentPtr chartData = instrNeedingScan_->chartData();

    // Abort if list is obsolete - This can happen if the app shuts down, or the user switches directories
    // for pattern scanning. In either case, we need to exit this task & thread as soon as possible.
    if(instrList.listIsObsolete()) return;

    PeriodValCltnIterListPtr pivotHighBeginIters = PivotHighScanner().scanPivotHighBeginIters(chartData);

    if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     PatternScannerPtr doubleBottomScanner(new DoubleBottomScanner(DoubleRange(7.0,40.0)));
     MultiPatternScanner multiScanner(doubleBottomScanner);
     PatternMatchListPtr doubleBottoms = multiScanner.scanUniquePatternMatches(chartData,pivotHighBeginIters);
     currentPatternMatches->insert(currentPatternMatches->end(),doubleBottoms->begin(),doubleBottoms->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     SymetricTriangleScanner wedgeScanner;
     PatternMatchListPtr symetricTriangles = wedgeScanner.scanPatternMatches(chartData);
     currentPatternMatches->insert(currentPatternMatches->end(),symetricTriangles->begin(),symetricTriangles->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     DescendingTriangleScanner descTriangleScanner;
     PatternMatchListPtr descTriangle = descTriangleScanner.scanPatternMatches(chartData);
     currentPatternMatches->insert(currentPatternMatches->end(),descTriangle->begin(),descTriangle->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     RectangleScanner flatBaseScanner;
     PatternMatchListPtr flatBases = flatBaseScanner.scanPatternMatches(chartData);
     currentPatternMatches->insert(currentPatternMatches->end(),flatBases->begin(),flatBases->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     RisingWedgeScanner risingWedgeScanner;
     PatternMatchListPtr risingWedges = risingWedgeScanner.scanPatternMatches(chartData);
     currentPatternMatches->insert(currentPatternMatches->end(),risingWedges->begin(),risingWedges->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     FallingWedgeScanner fallingWedgeScanner;
     PatternMatchListPtr fallingWedges = fallingWedgeScanner.scanPatternMatches(chartData);
     currentPatternMatches->insert(currentPatternMatches->end(),fallingWedges->begin(),fallingWedges->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     AscendingTriangleScanner ascendingTriangleScanner;
     PatternMatchListPtr ascTriangles = ascendingTriangleScanner.scanPatternMatches(chartData);
     currentPatternMatches->insert(currentPatternMatches->end(),ascTriangles->begin(),ascTriangles->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

     PatternScannerPtr cupWithoutHandleScanner(new CupWithoutHandleScanner());
     MultiPatternScanner multiCupScanner(cupWithoutHandleScanner);
     PatternMatchListPtr cupWithoutHandleMatches = multiCupScanner.scanUniquePatternMatches(chartData,pivotHighBeginIters);
     currentPatternMatches->insert(currentPatternMatches->end(),cupWithoutHandleMatches->begin(),cupWithoutHandleMatches->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

      PatternScannerPtr cupWithHandleScanner(new CupWithHandleScanner());
     MultiPatternScanner multiCupWithHandleScanner(cupWithHandleScanner);
     PatternMatchListPtr cupWithHandleMatches = multiCupWithHandleScanner.scanUniquePatternMatches(chartData,pivotHighBeginIters);
     currentPatternMatches->insert(currentPatternMatches->end(),cupWithHandleMatches->begin(),cupWithHandleMatches->end());

     if(instrList.listIsObsolete()) return; // Abort if list is obsolete

    instrNeedingScan_->setPatternMatches(currentPatternMatches);

    instrList.oneInstrScanComplete();
}
