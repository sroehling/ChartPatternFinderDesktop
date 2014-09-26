#include "InstrumentListTableView.h"
#include <QDir>
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>
#include "InstrumentSelectionInfo.h"
#include <assert.h>

InstrumentListTableView::InstrumentListTableView() :
    QTableView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

    // Hide the row numbers
    verticalHeader()->setVisible(false);

}


void InstrumentListTableView::populateFromCSVFiles(QString quoteFilePath)
{
    QDir dir = QDir(quoteFilePath);

    //Opens the path
    dir.setNameFilters(QStringList("*.csv"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    qDebug() << "Scanning: " << dir.path();
    QStringList fileList = dir.entryList();

    unsigned int numRows = fileList.count();
    unsigned int numCols = 1;

    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);
    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Instrument/Symbol"));

    for (int rowNum=0; rowNum<fileList.count(); rowNum++)
    {
        qDebug() << "Found file (full path): " << dir.absoluteFilePath(fileList[rowNum]);
        qDebug() << "Found file: " << fileList[rowNum];

        InstrumentSelectionInfoPtr instrSelInfo(new InstrumentSelectionInfo(dir,fileList[rowNum]));

        unsigned int colNum = 0;
        tableModel->setItem(rowNum,colNum,new QStandardItem(instrSelInfo->instrumentName()));

        instrumentInfo_.push_back(instrSelInfo);
    }

    setModel(tableModel);

    // Automatically resize the columns to match the width of the table view.
    for (int colNum = 0; colNum < horizontalHeader()->count(); colNum++)
    {
        horizontalHeader()->setSectionResizeMode(colNum, QHeaderView::Stretch);
    }

    // selectionModel() will be NULL until the tableModel is set.
    connect(selectionModel(), SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
              this, SLOT(instrumentSelectionChanged(const QItemSelection &,const QItemSelection &)));

    if(instrumentInfo_.size()>0)
    {
        // initially select the first instrument. This must happen after the slot connection
        // is established, since this will cause instrumentSelectionChanged() to be called.
        selectRow(0);
    }
}

void InstrumentListTableView::selectInstrument(int instrNum)
{
    assert(instrNum >=0);
    assert((unsigned int)instrNum < instrumentInfo_.size());

    InstrumentSelectionInfoPtr selInfo = instrumentInfo_[instrNum];

    emit instrumentSelected(selInfo);

}

void InstrumentListTableView::instrumentSelectionChanged (const QItemSelection  &,
                                      const QItemSelection  & )
{
    unsigned int currentRow = selectionModel()->selectedRows().first().row();  //QModelIndexList is an ordered list
    selectInstrument(currentRow);
}
