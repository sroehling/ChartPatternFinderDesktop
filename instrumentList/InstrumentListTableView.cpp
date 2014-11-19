#include "InstrumentListTableView.h"
#include <QDir>
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>
#include "InstrumentSelectionInfo.h"
#include <assert.h>
#include "InstrumentListWorker.h"
#include <QDebug>
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
    connect(instrumentList_.get(),SIGNAL(instrumentScanProgressUpdated(unsigned int)),
            this,SLOT(instrListScanProgressUpdated(unsigned int)));
    connect(instrumentList_.get(),SIGNAL(instrScanComplete()),
            this,SLOT(instrListScanComplete()));

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

void InstrumentListTableView::instrListScanProgressUpdated(unsigned int numInstrsScanned)
{
    emit instrumentScanProgressUpdated(numInstrsScanned);
}

void InstrumentListTableView::instrListScanComplete()
{
    qDebug() << "InstrumentListTableView::instrListScanComplete()";
    emit instrumentScanComplete();
}

void InstrumentListTableView::obsoleteCurrentInstrumentList()
{
    if(instrumentList_)
    {
        // If there's an existing list, "obsolete" the list, so any scanning which
        // is already in progress will stop.
        instrumentList_->obsoleteList();
        instrumentList_ = InstrumentListPtr(); // NULL Pointer
    }

}

void InstrumentListTableView::populateFromCSVFiles(QString quoteFilePath)
{
    obsoleteCurrentInstrumentList();
    instrumentList_ = InstrumentListPtr(new InstrumentList(quoteFilePath));

    initTable();

    emit startingPatternScan(instrumentList_->numCSVFiles());

    // Add a couple worker threads to read (and validate) the quote data then scan patterns for each
    // of the instruments (symbols/tickers).
    int idealThreads = QThread::idealThreadCount();
    if(idealThreads > 0)
    {
        // Leave 1 "ideal thread" for the main thread.
        int scannerThreads = std::max(1,idealThreads-1);
        qDebug() << "Thread count for scanning: " << scannerThreads;
        for(int i = 0; i < scannerThreads; i++)
        {
            QThreadPool::globalInstance()->start(new InstrumentListWorker(instrumentList_));
        }
    }
    else
    {
        QThreadPool::globalInstance()->start(new InstrumentListWorker(instrumentList_));
    }
}

void InstrumentListTableView::appExitCleanupHandler()
{
    obsoleteCurrentInstrumentList();
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
