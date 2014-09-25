#include "PatternMatchTableView.h"
#include "PatternMatch.h"
#include "QDateHelper.h"
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDebug>
#include <QHeaderView>
#include "PatternMatchFilter.h"

using namespace patternMatchFilter;

PatternMatchTableView::PatternMatchTableView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

    // Hide the row numbers
    verticalHeader()->setVisible(false);


}

void PatternMatchTableView::populatePatternMatches(const PatternMatchListPtr &patternMatches)
{
    PatternMatchListPtr sortedMatches = sortPatternMatches(patternMatches,ReverseSortPatternMatchByEndTimeThenLength());
    currentPatternMatches_ = sortedMatches;

    unsigned int numRows = patternMatches->size();
    unsigned int numCols = 5;
    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);

    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Pattern Type"));
    tableModel->setHorizontalHeaderItem(1,new QStandardItem("Start"));
    tableModel->setHorizontalHeaderItem(2,new QStandardItem("Finish"));
    tableModel->setHorizontalHeaderItem(3,new QStandardItem("Length (periods)"));
    tableModel->setHorizontalHeaderItem(4,new QStandardItem("High"));
    tableModel->setHorizontalHeaderItem(5,new QStandardItem("Low"));
    tableModel->setHorizontalHeaderItem(6,new QStandardItem("Depth %"));

    // Other possible columns: high, low, confirmation



    unsigned int rowNum = 0;
    for(PatternMatchList::const_iterator matchIter = sortedMatches->begin();
        matchIter != sortedMatches->end(); matchIter++)
    {
        unsigned int colNum = 0;

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString((*matchIter)->matchType().c_str())));
        colNum++;

        QDateTime startTime = QDateHelper::boostToQDateTime((*matchIter)->startTime());
        QString dateFormat = "yyyy-MM-dd";
        tableModel->setItem(rowNum,colNum,new QStandardItem(startTime.toString(dateFormat)));
        colNum++;

        QDateTime endTime = QDateHelper::boostToQDateTime((*matchIter)->endTime());
        tableModel->setItem(rowNum,colNum,new QStandardItem(endTime.toString(dateFormat)));
        colNum++;
/*
        if((*matchIter)->isIncompleteMatch())
        {
            tableModel->setItem(rowNum,colNum,new QStandardItem("Incomplete"));

        }
        else
        {
            QDateTime endTime = QDateHelper::boostToQDateTime((*matchIter)->endTime());
            tableModel->setItem(rowNum,colNum,new QStandardItem(endTime.toString(dateFormat)));
        }
  */

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString().sprintf("%d",(*matchIter)->numPeriods())));
        colNum++;

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString().sprintf("%.2f",(*matchIter)->highestHigh())));
        colNum++;

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString().sprintf("%.2f",(*matchIter)->lowestLow())));
        colNum++;

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString().sprintf("%.1f%%",(*matchIter)->depthPercent())));
        colNum++;

        rowNum++;
    }

    setModel(tableModel);


    // The first column always needs to show the entire pattern type. The remaining
    // columns can stretch to the size of the window/grid.
    horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    // Automatically resize the remaining columns to match the width of the table view.
    for (int colNum = 1; colNum < horizontalHeader()->count(); colNum++)
    {
        horizontalHeader()->setSectionResizeMode(colNum, QHeaderView::Stretch);
    }


    if(patternMatches->size() > 0)
    {
        selectRow(0);
    }
    connect(selectionModel(), SIGNAL(selectionChanged (const QItemSelection&, const QItemSelection&)),
              this, SLOT(patternTableSelectionChanged(const QItemSelection &,const QItemSelection &)));

}

void PatternMatchTableView::patternTableSelectionChanged (const QItemSelection  &selected,
                                      const QItemSelection  & )
{
    qDebug() << "Pattern Table Selection: " << selected;
    unsigned int currentRow = selectionModel()->selectedRows().first().row();  //QModelIndexList is an ordered list
    qDebug() << "Pattern Table Selection: selected row =  " << currentRow;

    assert(currentRow< currentPatternMatches_->size());

    PatternMatchList::iterator matchListIter = currentPatternMatches_->begin();
    std::advance(matchListIter,currentRow);
    PatternMatchPtr currMatch = (*matchListIter);

    emit patternMatchSelected(currMatch);
}

