#include "InstrumentListTableView.h"
#include <QDir>
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>
#include "InstrumentSelectionInfo.h"
#include <assert.h>
#include "InstrumentListWorker.h"

#include <QThreadPool>

InstrumentListTableView::InstrumentListTableView() :
    QTableView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

    // Hide the row numbers
    verticalHeader()->setVisible(false);

}


void InstrumentListTableView::initTable()
{
    // This will initialize the table, but wait for calls to instrumentAddedToList
    // to populate the rows.
    assert(instrumentList_);

    tableModel_ = new QStandardItemModel(this);
    tableModel_->setHorizontalHeaderItem(0,new QStandardItem("Instrument/Symbol"));

    setModel(tableModel_);

    // Automatically resize the columns to match the width of the table view.
    for (int colNum = 0; colNum < horizontalHeader()->count(); colNum++)
    {
        horizontalHeader()->setSectionResizeMode(colNum, QHeaderView::Stretch);
    }

    // selectionModel() will be NULL until the tableModel is set.
    connect(selectionModel(), SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
              this, SLOT(instrumentSelectionChanged(const QItemSelection &,const QItemSelection &)));
    connect(instrumentList_.get(),SIGNAL(instrumentAdded(unsigned int)),
            this,SLOT(instrumentAddedToList(unsigned int)));

}

void InstrumentListTableView::instrumentAddedToList(unsigned int instrNum)
{
    InstrumentSelectionInfoPtr instrSelInfo = instrumentList_->instrInfo(instrNum);

    unsigned int colNum = 0;
    unsigned int rowNum = instrNum;
    tableModel_->setItem(rowNum,colNum,new QStandardItem(instrSelInfo->instrumentName()));

    if(instrNum == 0)
    {
        // initially select the first instrument. This must happen after the slot connection
        // is established, since this will cause instrumentSelectionChanged() to be called.
        selectRow(0);
    }
}

void InstrumentListTableView::populateFromCSVFiles(QString quoteFilePath)
{
    if(instrumentList_)
    {
        // If there's an existing list, "obsolete" the list, so any scanning which
        // is already in progress will stop.
        instrumentList_->obsoleteList();
    }
    instrumentList_ = InstrumentListPtr(new InstrumentList(quoteFilePath));

    initTable();

    // Add a couple worker threads to read (and validate) the quote data then scan patterns for each
    // of the instruments (symbols/tickers).
    QThreadPool::globalInstance()->start(new InstrumentListWorker(instrumentList_));
    QThreadPool::globalInstance()->start(new InstrumentListWorker(instrumentList_));
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
