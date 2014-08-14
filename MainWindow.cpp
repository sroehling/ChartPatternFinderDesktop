#include "MainWindow.h"
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plot.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QStandardItemModel>
#include <QItemSelectionModel>

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

    QTableView* patternTable = new QTableView();
    unsigned int numRows = 2;
    patternTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    patternTable->setSelectionMode(QAbstractItemView::SingleSelection);;
    unsigned int numCols = 3;
    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);
    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Pattern Type"));
    tableModel->setHorizontalHeaderItem(1,new QStandardItem("Length"));
    tableModel->setHorizontalHeaderItem(2,new QStandardItem("Depth"));
    tableModel->setItem(0,0,new QStandardItem(QString("Double Bottom")));
    tableModel->setItem(1,0,new QStandardItem(QString("Cup with Handle")));

//   QItemSelectionModel *tableSelectionModel = new QItemSelectionModel(tableModel);
 //   patternTable->setSelectionModel(tableSelectionModel);
    patternTable->setModel(tableModel);

    connect(patternTable->selectionModel(), SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
              this, SLOT(patternTableSelectionChanged(const QItemSelection &,const QItemSelection &)));
    qDebug() << "pattern table setup complete";


    d_plot = new Plot( this );
  //  setCentralWidget( d_plot );
    d_plot->setMode( typeBox->currentIndex() );
    connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
        d_plot, SLOT( setMode( int ) ) );



    QVBoxLayout *mainWindowLayout = new QVBoxLayout;
    mainWindowLayout->addWidget(d_plot);
    mainWindowLayout->addWidget(patternTable);

    QWidget *centralWindow = new QWidget();
    centralWindow->setLayout(mainWindowLayout);
    setCentralWidget(centralWindow);

    d_plot->populate();

}

MainWindow::~MainWindow()
{
    // TBD - Should the destructor delete d_plot or does the main window take ownership?
 //   delete d_plot;
}


void MainWindow::patternTableSelectionChanged (const QItemSelection  &selected,
                                      const QItemSelection  & )
{
    qDebug() << "Pattern Table Selection: " << selected;

}
