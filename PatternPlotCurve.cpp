
#include <qwt_symbol.h>

#include "PatternPlotCurve.h"
#include "QDateHelper.h"
#include "PatternShapeGenerator.h"
#include "PatternMatch.h"
#include "PatternShape.h"


PatternPlotCurve::PatternPlotCurve(const PatternShapePointVectorPtr &shapeCurve, bool doCurveFit)
{

    setTitle( "Pattern" );
    setPen( Qt::blue, 3 ),
    setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QPolygonF shapePoints;
    for(PatternShapePointVector::iterator shapeIter = shapeCurve->begin();
        shapeIter != shapeCurve->end(); shapeIter++)
    {
        shapePoints << QPointF( (*shapeIter).pseudoXVal(), (*shapeIter).periodVal()
                           );
    }

    setCurveAttribute(QwtPlotCurve::Fitted,doCurveFit);
    setSamples( shapePoints );

}
