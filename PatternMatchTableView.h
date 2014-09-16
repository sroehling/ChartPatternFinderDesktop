#ifndef PATTERNMATCHTABLEVIEW_H
#define PATTERNMATCHTABLEVIEW_H

#include <QTableView>
#include "PatternMatch.h"

class PatternMatchTableView : public QTableView
{
    Q_OBJECT

public:
    explicit PatternMatchTableView();
    void populatePatternMatches(const PatternMatchListPtr &patternMatches);
    const PatternMatchListPtr currentPatternMatches() const { return currentPatternMatches_; }
signals:
    void patternMatchSelected(const PatternMatchPtr &);

public slots:
    void patternTableSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  & );

private:
    PatternMatchListPtr currentPatternMatches_;

};

#endif // PATTERNMATCHTABLEVIEW_H
