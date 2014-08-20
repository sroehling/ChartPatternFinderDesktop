#include "InstrumentListTableView.h"
#include <QDir>
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>


InstrumentListTableView::InstrumentListTableView() :
    QTableView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

}


void InstrumentListTableView::populateFromCSVFiles(QString quoteFilePath)
{
    dir_ = QDir(quoteFilePath);

    //Opens the path
    dir_.setNameFilters(QStringList("*.csv"));
    dir_.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    qDebug() << "Scanning: " << dir_.path();
    fileList_ = dir_.entryList();

    unsigned int numRows = fileList_.count();
    unsigned int numCols = 1;

    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);
    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Instrument/Symbol"));

    for (int rowNum=0; rowNum<fileList_.count(); rowNum++)
    {
        unsigned int colNum = 0;
        tableModel->setItem(rowNum,colNum,new QStandardItem(QString(fileList_[rowNum])));
        qDebug() << "Found file (full path): " << dir_.absoluteFilePath(fileList_[rowNum]);
        qDebug() << "Found file: " << fileList_[rowNum];
    }

    setModel(tableModel);

    // Automatically resize the columns to match the width of the table view.
    for (int colNum = 0; colNum < horizontalHeader()->count(); colNum++)
    {
        horizontalHeader()->setSectionResizeMode(colNum, QHeaderView::Stretch);
    }

 //   resizeColumnsToContents();

    // selectionModel() will be NULL until the tableModel is set.
    connect(selectionModel(), SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
              this, SLOT(instrumentSelectionChanged(const QItemSelection &,const QItemSelection &)));
}

void InstrumentListTableView::instrumentSelectionChanged (const QItemSelection  &,
                                      const QItemSelection  & )
{
    unsigned int currentRow = selectionModel()->selectedRows().first().row();  //QModelIndexList is an ordered list
    qDebug() << "Instrument List Table Selection: " << fileList_[currentRow];

    emit instrumentSelected(dir_.absoluteFilePath(fileList_[currentRow]));
}
