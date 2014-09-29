#include "InstrumentListTableView.h"
#include <QDir>
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>
#include "InstrumentSelectionInfo.h"
#include <assert.h>

#include <QThreadPool>
#include "PatternScanWorkerTask.h"

InstrumentListTableView::InstrumentListTableView() :
    QTableView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

    // Hide the row numbers
    verticalHeader()->setVisible(false);

}


void InstrumentListTableView::populateTable()
{
    assert(instrumentList_);
    unsigned int numRows = instrumentList_->numInstruments();
    unsigned int numCols = 1;

    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);
    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Instrument/Symbol"));

    for (unsigned int rowNum=0; rowNum<numRows; rowNum++)
    {

        InstrumentSelectionInfoPtr instrSelInfo = instrumentList_->instrInfo(rowNum);

        unsigned int colNum = 0;
        tableModel->setItem(rowNum,colNum,new QStandardItem(instrSelInfo->instrumentName()));
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

}

void InstrumentListTableView::populateFromCSVFiles(QString quoteFilePath)
{

    if(instrumentList_)
    {
        // If there's an existing list, "obsolete" the list, so any scanning which
        // is already being performed will stop.
        instrumentList_->obsoleteList();
    }
    instrumentList_ = InstrumentListPtr(new InstrumentList(quoteFilePath));

    populateTable();

    if(instrumentList_->numInstruments()>0)
    {
        // initially select the first instrument. This must happen after the slot connection
        // is established, since this will cause instrumentSelectionChanged() to be called.
        selectRow(0);
    }

    QThreadPool::globalInstance()->start(new PatternScanWorkerTask(instrumentList_));
    QThreadPool::globalInstance()->start(new PatternScanWorkerTask(instrumentList_));
}

void InstrumentListTableView::selectInstrument(int instrNum)
{
    assert(instrNum >=0);
    assert((unsigned int)instrNum < instrumentList_->numInstruments());

    InstrumentSelectionInfoPtr selInfo = instrumentList_->instrInfoWithScannedPatterns(instrNum);

    emit instrumentSelected(selInfo);

}

void InstrumentListTableView::instrumentSelectionChanged (const QItemSelection  &,
                                      const QItemSelection  & )
{
    unsigned int currentRow = selectionModel()->selectedRows().first().row();  //QModelIndexList is an ordered list
    selectInstrument(currentRow);
}
