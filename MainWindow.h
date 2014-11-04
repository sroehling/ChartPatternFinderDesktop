#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plot.h"
#include <QItemSelectionModel>
#include <QTableView>
#include <QPushButton>
#include "PatternMatch.h"
#include "PatternMatchTableView.h"
#include "InstrumentListTableView.h"
#include "QSettings"
#include "StackedStockCharts.h"
#include "SettingsHelper.h"
#include "LicenseRegistration.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString chooseQuotesDir(bool requireChoice);

    QSettingsPtr appSettings_;
    LicenseRegistrationPtr LicenseRegistration_;

    Plot *priceAndPatternPlot_;
    StackedStockCharts *stackedStockCharts_;

    PatternMatchTableView* patternTable_;
    InstrumentListTableView *instrumentListTableView_;

    QPushButton *registerButton_;
    QPushButton *buyButton_;

    void configureUIForFullVersion();

private slots:
     void actionSelectQuotesDir();
     void actionRefreshQuotes();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo);
    void patternMatchesSelected(const PatternMatchListPtr &selectedMatch);
    void openHelpUrl();
    void openRegisterDialog();
    void licenseRegistrationComplete();
    void openBuyUrl();
    void appExitCleanupHandler();
};

#endif // MAINWINDOW_H
