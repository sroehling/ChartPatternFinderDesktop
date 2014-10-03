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
#include <QPushButton>
#include <QStyle>

#define APP_SETTINGS_KEY_QUOTES_DIR "QUOTES_DIR"

void MainWindow::initMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *selectQuotesDirAction = new QAction(tr("&Select Quotes Directory..."), this);
    fileMenu->addAction(selectQuotesDirAction);
    connect(selectQuotesDirAction, SIGNAL(triggered()), this, SLOT(actionSelectQuotesDir()));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    appSettings_ = QSettingsPtr(new QSettings(QString("configs/PatternRecogSettings.ini"), QSettings::IniFormat));

    QToolBar *toolBar = new QToolBar( this );

    QComboBox *typeBox = new QComboBox( toolBar );
    typeBox->addItem( "Bars" );
    typeBox->addItem( "CandleSticks" );
    typeBox->setCurrentIndex( 1 );
    typeBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QToolButton *btnExport = new QToolButton( toolBar );
    btnExport->setText( "Export" );
    btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
 //   connect( btnExport, SIGNAL( clicked() ), priceAndPatternPlot_, SLOT( exportPlot() ) );

    toolBar->addWidget( typeBox );
    toolBar->addWidget( btnExport );
    addToolBar( toolBar );

    patternTable_ = new PatternMatchTableView();
    instrumentListTableView_ = new InstrumentListTableView();

    stackedStockCharts_ = new StackedStockCharts(this);
    priceAndPatternPlot_ = stackedStockCharts_->priceAndPatternPlot();

    priceAndPatternPlot_->setMode( typeBox->currentIndex() );
    connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
        priceAndPatternPlot_, SLOT( setMode( int ) ) );

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


    QVBoxLayout *chartAndPatternListLayout = new QVBoxLayout();
    chartAndPatternListLayout->addWidget(stackedStockCharts_,30);
    chartAndPatternListLayout->addWidget(patternTable_,10);
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


    initMenus();

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
