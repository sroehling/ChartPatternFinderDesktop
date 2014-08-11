
#include <qwt_symbol.h>

#include "PatternPlotCurve.h"
#include "QDateHelper.h"
#include "PatternShapeGenerator.h"
#include "PatternMatch.h"

PatternPlotCurve::PatternPlotCurve(const PatternMatchPtr &patternMatch)
{

    setTitle( "Pattern" );
    setPen( Qt::blue, 2 ),
    setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 1 ), QSize( 4, 4 ) );

    setSymbol( symbol );

    PatternShapeGenerator shapeGen;
    PatternShapePointVectorPtr shapeCurve = shapeGen.generateShape(*patternMatch);

    QPolygonF shapePoints;
    for(PatternShapePointVector::iterator shapeIter = shapeCurve->begin();
        shapeIter != shapeCurve->end(); shapeIter++)
    {
        shapePoints << QPointF( QwtDate::toDouble(
                  QDateHelper::boostToQDateTime((*shapeIter).periodTime())), (*shapeIter).periodVal()
                           );
    }


    setSamples( shapePoints );

}
