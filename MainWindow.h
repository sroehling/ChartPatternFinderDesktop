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
#include "StackedStockCharts.h"

typedef boost::shared_ptr<QSettings> QSettingsPtr;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString chooseQuotesDir(bool requireChoice);

    QSettingsPtr appSettings_;

    Plot *priceAndPatternPlot_;
    StackedStockCharts *stackedStockCharts_;

    PatternMatchTableView* patternTable_;
    InstrumentListTableView *instrumentListTableView_;

private slots:
     void actionSelectQuotesDir();
     void refreshQuotes();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo);
    void patternMatchesSelected(const PatternMatchListPtr &selectedMatch);
    void openHelpUrl();
    void openRegisterDialog();
    void openBuyUrl();
};

#endif // MAINWINDOW_H
