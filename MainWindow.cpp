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
 //   connect( btnExport, SIGNAL( clicked() ), d_plot, SLOT( exportPlot() ) );

    toolBar->addWidget( typeBox );
    toolBar->addWidget( btnExport );
    addToolBar( toolBar );

    patternTable_ = new PatternMatchTableView();
    instrumentListTableView_ = new InstrumentListTableView();

    d_plot = new Plot( this );
    d_plot->setMode( typeBox->currentIndex() );
    connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
        d_plot, SLOT( setMode( int ) ) );

    QGridLayout *mainWindowGridLayout = new QGridLayout;
    mainWindowGridLayout->addWidget(instrumentListTableView_,0,0,2,1);
    mainWindowGridLayout->addWidget(d_plot,0,1,1,1);
    mainWindowGridLayout->addWidget(patternTable_,1,1,1,1);
    mainWindowGridLayout->setColumnStretch(0, 5);
    mainWindowGridLayout->setColumnStretch(1, 30);
    mainWindowGridLayout->setRowStretch(0,30);
    mainWindowGridLayout->setRowStretch(1,10);


    QWidget *centralWindow = new QWidget();
    centralWindow->setLayout(mainWindowGridLayout);
    setCentralWidget(centralWindow);

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

MainWindow::~MainWindow()
{
}

void MainWindow::patternMatchesSelected(const PatternMatchListPtr &selectedMatches)
{
    d_plot->populatePatternMatchesShapes(selectedMatches);
}

void MainWindow::instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo)
{

    assert(instrSelectionInfo->patternScanComplete());
    PatternMatchListPtr currentPatternMatches = instrSelectionInfo->patternMatches();

    d_plot->populateChartData(instrSelectionInfo);

    patternTable_->populatePatternMatches(currentPatternMatches);

    if(patternTable_->currentPatternMatches()->size() > 0)
    {
       patternTable_->selectRow(0);
       d_plot->clearPatternPlots();
       d_plot->populateOnePatternShape(patternTable_->currentPatternMatches()->front());
    }

}
