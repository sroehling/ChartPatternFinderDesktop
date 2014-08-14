#include "MainWindow.h"
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plot.h"

#include <QTableView>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "PeriodValSegment.h"
#include "DoubleBottomScanner.h"
#include "PatternShapeGenerator.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "SymetricWedgeScanner.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    QToolBar *toolBar = new QToolBar( this );

    QComboBox *typeBox = new QComboBox( toolBar );
    typeBox->addItem( "Bars" );
    typeBox->addItem( "CandleSticks" );
    typeBox->setCurrentIndex( 1 );
    typeBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QToolButton *btnExport = new QToolButton( toolBar );
    btnExport->setText( "Export" );
    btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    connect( btnExport, SIGNAL( clicked() ), d_plot, SLOT( exportPlot() ) );

    toolBar->addWidget( typeBox );
    toolBar->addWidget( btnExport );
    addToolBar( toolBar );

    patternTable_ = new QTableView();
    patternTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    patternTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    patternTable_->setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

    d_plot = new Plot( this );
    d_plot->setMode( typeBox->currentIndex() );
    connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
        d_plot, SLOT( setMode( int ) ) );

    QVBoxLayout *mainWindowLayout = new QVBoxLayout;
    mainWindowLayout->addWidget(d_plot);
    mainWindowLayout->addWidget(patternTable_);

    QWidget *centralWindow = new QWidget();
    centralWindow->setLayout(mainWindowLayout);
    setCentralWidget(centralWindow);

    // Layout is finished. Populate the pattern plot and pattern selectin table with some data.


    //   PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("/Users/sroehling/Development/workspace/PatternRecognitionDesktop/lib/PatternRecognitionLib/test/patternShape/QCOR_2013_2014_Weekly.csv");
      PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("/Users/sroehling/Development/workspace/PatternRecognitionDesktop/lib/PatternRecognitionLib/test/patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    currentPatternMatches_ = new PatternMatchList();

    PatternScannerPtr doubleBottomScanner(new DoubleBottomScanner(DoubleRange(7.0,40.0)));
    MultiPatternScanner multiScanner(doubleBottomScanner);
    PatternMatchListPtr doubleBottoms = multiScanner.scanUniquePatternMatches(chartData);
    currentPatternMatches_->insert(currentPatternMatches_->end(),doubleBottoms->begin(),doubleBottoms->end());

    SymetricWedgeScanner wedgeScanner;
    PatternMatchListPtr symetricTriangles = wedgeScanner.scanPatternMatches(chartData);
    currentPatternMatches_->insert(currentPatternMatches_->end(),symetricTriangles->begin(),symetricTriangles->end());

    d_plot->populateChartData(chartData);


    unsigned int numRows = currentPatternMatches_->size();
    unsigned int numCols = 5;
    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);
    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Pattern Type"));
    tableModel->setHorizontalHeaderItem(1,new QStandardItem("Start"));
    tableModel->setHorizontalHeaderItem(2,new QStandardItem("Finish"));
    tableModel->setHorizontalHeaderItem(3,new QStandardItem("Length"));
    tableModel->setHorizontalHeaderItem(4,new QStandardItem("Depth"));
    tableModel->setItem(0,0,new QStandardItem(QString("Double Bottom")));
    patternTable_->setModel(tableModel);

    connect(patternTable_->selectionModel(), SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
              this, SLOT(patternTableSelectionChanged(const QItemSelection &,const QItemSelection &)));

    if(currentPatternMatches_->size() > 0)
    {
        patternTable_->selectRow(0);
    }

}

MainWindow::~MainWindow()
{
     delete currentPatternMatches_;
}


void MainWindow::patternTableSelectionChanged (const QItemSelection  &selected,
                                      const QItemSelection  & )
{
    qDebug() << "Pattern Table Selection: " << selected;

    unsigned int currentRow = patternTable_->selectionModel()->selectedRows().first().row();  //QModelIndexList is an ordered list
    qDebug() << "Pattern Table Selection: selected row =  " << currentRow;

    assert(currentRow< currentPatternMatches_->size());
    assert(currentPatternMatches_ != NULL);

    PatternMatchList::iterator matchListIter = currentPatternMatches_->begin();
    std::advance(matchListIter,currentRow);
    PatternMatchPtr currMatch = (*matchListIter);
    d_plot->populatePatternShapes(currMatch);

}
