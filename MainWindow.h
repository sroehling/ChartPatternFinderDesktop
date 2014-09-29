#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plot.h"
#include <QItemSelectionModel>
#include <QTableView>
#include "PatternMatch.h"
#include "PatternMatchTableView.h"
#include "InstrumentListTableView.h"
#include "QSettings"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<QSettings> QSettingsPtr;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void initMenus();
    QString chooseQuotesDir(bool requireChoice);

    QSettingsPtr appSettings_;
    Plot *d_plot;
    PatternMatchTableView* patternTable_;
    InstrumentListTableView *instrumentListTableView_;

private slots:
     void actionSelectQuotesDir();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo);
    void patternMatchesSelected(const PatternMatchListPtr &selectedMatch);
};

#endif // MAINWINDOW_H
