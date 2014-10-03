#include "MainWindow.h"
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plot.h"

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

#define APP_SETTINGS_KEY_QUOTES_DIR "QUOTES_DIR"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    appSettings_ = QSettingsPtr(new QSettings(QString("configs/PatternRecogSettings.ini"), QSettings::IniFormat));

    patternTable_ = new PatternMatchTableView();
    instrumentListTableView_ = new InstrumentListTableView();

    stackedStockCharts_ = new StackedStockCharts(this);
    priceAndPatternPlot_ = stackedStockCharts_->priceAndPatternPlot();

    QVBoxLayout *lhsLayout = new QVBoxLayout();
    lhsLayout->addWidget(instrumentListTableView_);

    QPushButton *openQuotesButton = new QPushButton("Open Quotes");
    connect(openQuotesButton,SIGNAL(clicked()),this,SLOT(actionSelectQuotesDir()));
    openQuotesButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    lhsLayout->addWidget(openQuotesButton);

    QPushButton *refreshQuotesButton = new QPushButton("Reload Quotes");
    refreshQuotesButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    lhsLayout->addWidget(refreshQuotesButton);

    QWidget *lhsContent = new QWidget();
    lhsContent->setLayout(lhsLayout);

    QLabel *websiteLinkLabel = new QLabel();
    websiteLinkLabel->setOpenExternalLinks(true);
    websiteLinkLabel->setText("<a href=\"http://www.resultra.com\">www.resultra.com</a>");

    QLabel *helpLinkLabel = new QLabel();
    helpLinkLabel->setOpenExternalLinks(true);
    helpLinkLabel->setText("<a href=\"http://www.resultra.com\">help</a>");
    helpLinkLabel->setPixmap(QPixmap(":/icons/help-button"));

    QPushButton *helpButton = new QPushButton("Help");
    helpButton->setIcon(QPixmap(":/icons/help-button"));
    connect(helpButton,SIGNAL(clicked()),this,SLOT(openHelpUrl()));

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(websiteLinkLabel,50,Qt::AlignLeft);
    infoLayout->addWidget(helpButton,50,Qt::AlignRight);


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

    if(!appSettings_->contains(APP_SETTINGS_KEY_QUOTES_DIR))
    {
        bool requireDirChoice = true;
        chooseQuotesDir(requireDirChoice);
    }
    QString quoteFileDirName = appSettings_->value(APP_SETTINGS_KEY_QUOTES_DIR).toString();


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

void MainWindow::refreshQuotes()
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
     QDesktopServices::openUrl(QUrl("http://www.resultra.com", QUrl::TolerantMode));
}
