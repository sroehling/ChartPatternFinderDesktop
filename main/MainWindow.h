#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PriceAndPatternPlot.h"
#include <QItemSelectionModel>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include "PatternMatch.h"
#include "PatternMatchTableView.h"
#include "InstrumentListTableView.h"
#include "QSettings"
#include "StackedStockCharts.h"
#include "SettingsHelper.h"
#include "WelcomeDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString chooseQuotesDir(bool requireChoice);

    QSettingsPtr appSettings_;

    PriceAndPatternPlot *priceAndPatternPlot_;
    StackedStockCharts *stackedStockCharts_;

    PatternMatchTableView* patternTable_;
    InstrumentListTableView *instrumentListTableView_;

    QProgressBar *scanningProgress_;
    QLabel *scanLabel_;

    QPushButton *registerButton_;
    QPushButton *buyButton_;

    WelcomeDialog *welcomeDialog_;

    void configureUIForFullVersion();

private slots:
     void actionSelectQuotesDir();
     void actionRefreshQuotes();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
     void quitApplication();
public slots:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo);
    void patternMatchesSelected(const PatternMatchListPtr &selectedMatch);
    void openHelpUrl();
    void licenseRegistrationComplete();
    void openBuyUrl();
    void appExitCleanupHandler();
    void welcomeDialogSelectQuotesConfirmed();
    void welcomeDialogQuitAppConfirmed();
    void startingPatternScan(unsigned int numInstrumentsToBeScanned);
    void instrumentScanProgressUpdated(unsigned int numInstrumentsScanned);
    void instrumentScanComplete();

};

#endif // MAINWINDOW_H
