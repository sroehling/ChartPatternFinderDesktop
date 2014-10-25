#ifndef PATTERNMATCHTABLEVIEW_H
#define PATTERNMATCHTABLEVIEW_H

#include <QTableView>
#include "PatternMatch.h"
#include "LicenseRegistration.h"

class PatternMatchTableView : public QTableView
{
    Q_OBJECT

private:
    LicenseRegistrationPtr licenseRegistration_;
public:
    explicit PatternMatchTableView(const LicenseRegistrationPtr &licenseRegistration);
    void populatePatternMatches(const PatternMatchListPtr &patternMatches);
    const PatternMatchListPtr currentPatternMatches() const { return currentPatternMatches_; }
signals:
    void patternMatchesSelected(const PatternMatchListPtr &);

public slots:
    void patternTableSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  & );

private:
    PatternMatchListPtr currentPatternMatches_;

};

#endif // PATTERNMATCHTABLEVIEW_H
