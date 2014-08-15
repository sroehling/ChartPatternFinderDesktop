#ifndef PATTERNMATCHTABLEVIEW_H
#define PATTERNMATCHTABLEVIEW_H

#include <QTableView>
#include "PatternMatch.h"

class PatternMatchTableView : public QTableView
{
    Q_OBJECT

public:
    explicit PatternMatchTableView();
    void populatePatternMatches(const PatternMatchList &patternMatches);

signals:

public slots:

};

#endif // PATTERNMATCHTABLEVIEW_H
