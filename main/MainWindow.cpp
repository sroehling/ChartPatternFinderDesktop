#include "MainWindow.h"
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "PriceAndPatternPlot.h"
#include "WelcomeDialog.h"

#include <QTableView>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include "PatternMatchTableView.h"
#include <QFileDialog>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>

#include "PeriodValSegment.h"
#include "PatternShapeGenerator.h"
#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "StackedStockCharts.h"
#include <QDesktopServices>
#include <QPushButton>
#include <QStyle>
#include <QLabel>
#include <QProgressBar>
#include "SettingsHelper.h"

#define APP_SETTINGS_KEY_QUOTES_DIR "QUOTES_DIR"
#define APP_NAME_TITLE "Chart Pattern Finder"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowTitle(APP_NAME_TITLE);

    appSettings_ = settingsHelper::openUserSettings();
    qDebug() << "Application settings file: " << appSettings_->fileName();


    // TBD - Figure out what to do with license registration
 //   connect(LicenseRegistration_.get(), SIGNAL(licenseRegistrationComplete()),
   //           this, SLOT(licenseRegistrationComplete()));

    patternTable_ = new PatternMatchTableView();
    instrumentListTableView_ = new InstrumentListTableView();

    stackedStockCharts_ = new StackedStockCharts(this);
    priceAndPatternPlot_ = stackedStockCharts_->priceAndPatternPlot();

    QVBoxLayout *lhsLayout = new QVBoxLayout();
    lhsLayout->addWidget(instrumentListTableView_);

    QPushButton *openQuotesButton = new QPushButton("Open Quotes");
    connect(openQuotesButton,SIGNAL(clicked()),this,SLOT(actionSelectQuotesDir()));
    openQuotesButton->setIcon(QPixmap(":/icons/load-quotes"));
    lhsLayout->addWidget(openQuotesButton);

    QPushButton *refreshQuotesButton = new QPushButton("Reload Quotes");
    refreshQuotesButton->setIcon(QPixmap(":/icons/reload-quotes"));
    lhsLayout->addWidget(refreshQuotesButton);
    connect(refreshQuotesButton,SIGNAL(clicked()),this,SLOT(actionRefreshQuotes()));


    // Label and progress indicator for pattern scanning.
    scanLabel_ = new QLabel("Scanning for Patterns:");
    scanningProgress_ = new QProgressBar(this);
    scanningProgress_->setMaximum(100);
    scanningProgress_->setValue(0);
    connect(instrumentListTableView_,SIGNAL(startingPatternScan(unsigned int)),this,SLOT(startingPatternScan(unsigned int)));
    connect(instrumentListTableView_,SIGNAL(instrumentScanProgressUpdated(unsigned int)),this,SLOT(instrumentScanProgressUpdated(unsigned int)));
    connect(instrumentListTableView_,SIGNAL(instrumentScanComplete()),this,SLOT(instrumentScanComplete()));

    QWidget *lhsContent = new QWidget();
    lhsContent->setLayout(lhsLayout);

    QLabel *websiteLinkLabel = new QLabel();
    websiteLinkLabel->setOpenExternalLinks(true);
    websiteLinkLabel->setText("<a href=\"https://www.chartpatternfinder.com\">ChartPatternFinder.com</a>");

    // Help Button
    QLabel *helpLinkLabel = new QLabel();
    helpLinkLabel->setOpenExternalLinks(true);
    helpLinkLabel->setText("<a href=\"https://www.chartpatternfinder.com/help\">help</a>");
    helpLinkLabel->setPixmap(QPixmap(":/icons/help-button"));

    QPushButton *helpButton = new QPushButton("Help");
    helpButton->setIcon(QPixmap(":/icons/help-button"));
    connect(helpButton,SIGNAL(clicked()),this,SLOT(openHelpUrl()));

    buyButton_ = new QPushButton("Buy Full Version");
    buyButton_->setIcon(QPixmap(":/icons/upgrade-button"));
    connect(buyButton_,SIGNAL(clicked()),this,SLOT(openBuyUrl()));

    registerButton_ = new QPushButton("Register ...");
    registerButton_->setIcon(QPixmap(":/icons/register-button"));
  //  connect(registerButton_,SIGNAL(clicked()),this,SLOT(openRegisterDialog()));

    // Main Layout
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(scanLabel_,Qt::AlignLeft);
    infoLayout->addSpacerItem(new QSpacerItem(5,5));
    infoLayout->addWidget(scanningProgress_,Qt::AlignLeft);
    infoLayout->addSpacerItem(new QSpacerItem(10,10));
    infoLayout->addWidget(websiteLinkLabel,100,Qt::AlignRight);
    infoLayout->addSpacerItem(new QSpacerItem(10,10));
    infoLayout->addWidget(buyButton_,0,Qt::AlignRight);
    infoLayout->addSpacerItem(new QSpacerItem(10,10));
    infoLayout->addWidget(registerButton_,0,Qt::AlignRight);
    infoLayout->addSpacerItem(new QSpacerItem(10,10));
    infoLayout->addWidget(helpButton,0,Qt::AlignRight);

    QVBoxLayout *chartAndPatternListLayout = new QVBoxLayout();
    chartAndPatternListLayout->setSpacing(0);
    chartAndPatternListLayout->addWidget(stackedStockCharts_,30);
    chartAndPatternListLayout->addWidget(patternTable_,10);
    chartAndPatternListLayout->addLayout(infoLayout,0);
    QWidget *rhsContent = new QWidget();
    rhsContent->setLayout(chartAndPatternListLayout);


    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(lhsContent);
    mainSplitter->addWidget(rhsContent);
    mainSplitter->setStretchFactor(0,30);
    mainSplitter->setStretchFactor(1,70);
    mainSplitter->setCollapsible(0,false);
    mainSplitter->setCollapsible(1,false);

    setCentralWidget(mainSplitter);

    welcomeDialog_ = new WelcomeDialog(this);
    connect(welcomeDialog_,SIGNAL(welcomeDialogSelectQuotesButtonPressed()),
            this, SLOT(welcomeDialogSelectQuotesConfirmed()));
    connect(welcomeDialog_,SIGNAL(welcomeDialogQuitButtonPressed()),
            this, SLOT(welcomeDialogQuitAppConfirmed()));

    if(!appSettings_->contains(APP_SETTINGS_KEY_QUOTES_DIR))
    {
        // If this is the first time starting up the application, show the welcome dialog. This
        // Welcome dialog will in turn prompt the user to select a quotes folder, or allow the
        // user to quit and come back to do so.
        //
        // The welcome dialog has a connection back to the main window (see above) when either the
        // "Select Quotes" button or "Quit" button is pressed. This will bring up the prompt for
        // the quotes directory or quit the application accordingly.
        welcomeDialog_->show();
     }
    QString quoteFileDirName = appSettings_->value(APP_SETTINGS_KEY_QUOTES_DIR).toString();


    configureUIForFullVersion();

    // Layout is finished. Populate the pattern plot and pattern selectin table with some data.

    connect(patternTable_, SIGNAL(patternMatchesSelected (const PatternMatchListPtr &)),
              this, SLOT(patternMatchesSelected(const PatternMatchListPtr &)));
    connect(instrumentListTableView_, SIGNAL(instrumentSelected (const InstrumentSelectionInfoPtr &)),
              this, SLOT(instrumentSelected(const InstrumentSelectionInfoPtr &)));

    // The last thing to do during initialization is to set the quotes directory on the instrument list.
    // This needs to happen after connecting the slots, since this will initially select the first
    // instrument in the list, emitting the instrumentSelected signal from the instrument list.
    instrumentListTableView_->populateFromCSVFiles(quoteFileDirName);
}

QString MainWindow::chooseQuotesDir(bool requireChoice)
{
    QString quoteFileDirName = QFileDialog::getExistingDirectory(this, "Select Quotes Directory");
    if(requireChoice)
    {
       while(quoteFileDirName.isNull()) {
            quoteFileDirName = QFileDialog::getExistingDirectory(this, "Select Quotes Directory");
       }
    }
    if(!quoteFileDirName.isNull())
    {
        // Only propagate the selection to the app settings if a new selection was made.
        appSettings_->setValue(APP_SETTINGS_KEY_QUOTES_DIR,QVariant(quoteFileDirName));
    }
    return quoteFileDirName;
}

void MainWindow::actionSelectQuotesDir()
{
    bool requireDirChoice = false;
    QString quoteFileDirName = chooseQuotesDir(requireDirChoice);

    if(!quoteFileDirName.isNull())
    {
        // Only update the instrument list if a new choice was made
        instrumentListTableView_->populateFromCSVFiles(quoteFileDirName);
    }
}

void MainWindow::actionRefreshQuotes()
{
    assert(appSettings_->contains(APP_SETTINGS_KEY_QUOTES_DIR));
    QString quoteFileDirName = appSettings_->value(APP_SETTINGS_KEY_QUOTES_DIR).toString();
    instrumentListTableView_->populateFromCSVFiles(quoteFileDirName);
}

MainWindow::~MainWindow()
{
}

void MainWindow::patternMatchesSelected(const PatternMatchListPtr &selectedMatches)
{
    priceAndPatternPlot_->populatePatternMatchesShapes(selectedMatches);
}

void MainWindow::instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo)
{
    assert(instrSelectionInfo->patternScanComplete());
    PatternMatchListPtr currentPatternMatches = instrSelectionInfo->patternMatches();

    stackedStockCharts_->populateChartData(instrSelectionInfo);

    patternTable_->populatePatternMatches(currentPatternMatches);

    if(patternTable_->currentPatternMatches()->size() > 0)
    {
       patternTable_->selectRow(0);
       priceAndPatternPlot_->clearPatternPlots();
       priceAndPatternPlot_->populateOnePatternShape(patternTable_->currentPatternMatches()->front());
    }

}

void MainWindow::openHelpUrl()
{
     QDesktopServices::openUrl(QUrl("http://www.chartpatternfinder.com/support", QUrl::TolerantMode));
}

void MainWindow::openBuyUrl()
{
     QDesktopServices::openUrl(QUrl("http://www.chartpatternfinder.com/buy", QUrl::TolerantMode));
}

void MainWindow::configureUIForFullVersion()
{
    registerButton_->hide();
    buyButton_->hide();
}


void MainWindow::licenseRegistrationComplete()
{
    qDebug() << "Main Window: License Registration complete";
    configureUIForFullVersion();
}

void MainWindow::welcomeDialogSelectQuotesConfirmed()
{
    qDebug() << "Main Window: welcomeDialogSelectQuotesConfirmed()";
    assert(welcomeDialog_ != NULL);
    welcomeDialog_->close();

    bool requireDirChoice = true;
    chooseQuotesDir(requireDirChoice);


}


void MainWindow::startingPatternScan(unsigned int numInstrumentsToBeScanned)
{
    qDebug() << "Main Window: starting pattern scan";
    scanningProgress_->setMaximum(numInstrumentsToBeScanned);
    scanningProgress_->setValue(0);
    scanningProgress_->show();
    scanLabel_->show();

}

void MainWindow::instrumentScanProgressUpdated(unsigned int numInstrumentsScanned)
{
    scanningProgress_->setValue(numInstrumentsScanned);
}

void MainWindow::instrumentScanComplete()
{
    scanningProgress_->hide();
    scanLabel_->hide();
}

void MainWindow::welcomeDialogQuitAppConfirmed()
{
    welcomeDialog_->close();
    qDebug() << "Main Window: welcomeDialogQuitAppConfirmed()";
    emit quitApplication();
}

void MainWindow::appExitCleanupHandler()
{
    qDebug() << "Main Window: appExitCleanupHandler()";
    if(instrumentListTableView_)
    {
        instrumentListTableView_->appExitCleanupHandler();
    }

}
