#include "InstrumentSelectionInfo.h"
#include <QDir>

#include "PeriodValSegment.h"
#include "PatternMatch.h"
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


InstrumentSelectionInfo::InstrumentSelectionInfo(const QDir &directory,
                                                 const QString &fileName)
    : dir_(directory),
      fileName_(fileName)
{
    patternScanInProgress = false;
}



QString InstrumentSelectionInfo::instrumentName() const
{
    QString instrumentLabel(fileName_);
    instrumentLabel.replace(".csv","");
    return instrumentLabel;
}


const PeriodValSegmentPtr &InstrumentSelectionInfo::chartData()
{
    if(!chartData_)
    {
        QString instrFilePath(dir_.absoluteFilePath(fileName_));
        chartData_ = PeriodValSegment::readFromFile(instrFilePath.toStdString());
    }
    return chartData_;
}


void InstrumentSelectionInfo::scanPatternMatches()
{
    PatternMatchListPtr currentPatternMatches = PatternMatchListPtr(new PatternMatchList());

    PeriodValCltnIterListPtr pivotHighBeginIters = PivotHighScanner().scanPivotHighBeginIters(chartData());

     PatternScannerPtr doubleBottomScanner(new DoubleBottomScanner(DoubleRange(7.0,40.0)));
     MultiPatternScanner multiScanner(doubleBottomScanner);
     PatternMatchListPtr doubleBottoms = multiScanner.scanUniquePatternMatches(chartData(),pivotHighBeginIters);
     currentPatternMatches->insert(currentPatternMatches->end(),doubleBottoms->begin(),doubleBottoms->end());

     SymetricTriangleScanner wedgeScanner;
     PatternMatchListPtr symetricTriangles = wedgeScanner.scanPatternMatches(chartData());
     currentPatternMatches->insert(currentPatternMatches->end(),symetricTriangles->begin(),symetricTriangles->end());

     DescendingTriangleScanner descTriangleScanner;
     PatternMatchListPtr descTriangle = descTriangleScanner.scanPatternMatches(chartData());
     currentPatternMatches->insert(currentPatternMatches->end(),descTriangle->begin(),descTriangle->end());

     RectangleScanner flatBaseScanner;
     PatternMatchListPtr flatBases = flatBaseScanner.scanPatternMatches(chartData());

     currentPatternMatches->insert(currentPatternMatches->end(),flatBases->begin(),flatBases->end());

     RisingWedgeScanner risingWedgeScanner;
     PatternMatchListPtr risingWedges = risingWedgeScanner.scanPatternMatches(chartData());
     currentPatternMatches->insert(currentPatternMatches->end(),risingWedges->begin(),risingWedges->end());

     FallingWedgeScanner fallingWedgeScanner;
     PatternMatchListPtr fallingWedges = fallingWedgeScanner.scanPatternMatches(chartData());
     currentPatternMatches->insert(currentPatternMatches->end(),fallingWedges->begin(),fallingWedges->end());

     AscendingTriangleScanner ascendingTriangleScanner;
     PatternMatchListPtr ascTriangles = ascendingTriangleScanner.scanPatternMatches(chartData());
     currentPatternMatches->insert(currentPatternMatches->end(),ascTriangles->begin(),ascTriangles->end());

     PatternScannerPtr cupWithoutHandleScanner(new CupWithoutHandleScanner());
     MultiPatternScanner multiCupScanner(cupWithoutHandleScanner);
     PatternMatchListPtr cupWithoutHandleMatches = multiCupScanner.scanUniquePatternMatches(chartData(),pivotHighBeginIters);
     currentPatternMatches->insert(currentPatternMatches->end(),cupWithoutHandleMatches->begin(),cupWithoutHandleMatches->end());

      PatternScannerPtr cupWithHandleScanner(new CupWithHandleScanner());
     MultiPatternScanner multiCupWithHandleScanner(cupWithHandleScanner);
     PatternMatchListPtr cupWithHandleMatches = multiCupWithHandleScanner.scanUniquePatternMatches(chartData(),pivotHighBeginIters);
     currentPatternMatches->insert(currentPatternMatches->end(),cupWithHandleMatches->begin(),cupWithHandleMatches->end());


     patternMatches_ = currentPatternMatches;

}

const PatternMatchListPtr &InstrumentSelectionInfo::patternMatches()
{
    if(!patternMatches_)
    {
        scanPatternMatches();
    }
    return patternMatches_;
}

bool InstrumentSelectionInfo::patternScanComplete() const
{
    if(patternMatches_)
    {
        return true;
    }
    else
    {
        return false;
    }
}
