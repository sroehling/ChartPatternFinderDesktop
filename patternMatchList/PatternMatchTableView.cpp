#include "PatternMatchTableView.h"
#include "PatternMatch.h"
#include "QDateHelper.h"
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDebug>
#include <QHeaderView>
#include "PatternMatchFilter.h"
#include "LicenseRegistration.h"

using namespace patternMatchFilter;

PatternMatchTableView::PatternMatchTableView(const LicenseRegistrationPtr &licenseRegistration)
    : licenseRegistration_(licenseRegistration)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing

    // Hide the row numbers
    verticalHeader()->setVisible(false);
}

void PatternMatchTableView::populatePatternMatches(const PatternMatchListPtr &patternMatches)
{
    PatternMatchListPtr sortedMatches = sortPatternMatches(patternMatches,ReverseSortPatternMatchByEndTimeThenLength());

    // If the app is registered in trial mode, then only show historical patterns; i.e., don't show the
    // most recent or still forming pattern.
    if(!licenseRegistration_->fullVersionLicenseRegistered())
    {
        if(sortedMatches->size() > 0)
        {
            sortedMatches->pop_front();
        }
    }

    currentPatternMatches_ = sortedMatches;

    unsigned int numRows = currentPatternMatches_->size();
    unsigned int numCols = 5;
    QStandardItemModel *tableModel = new QStandardItemModel(numRows, numCols,this);

    tableModel->setHorizontalHeaderItem(0,new QStandardItem("Pattern Type"));
    tableModel->setHorizontalHeaderItem(1,new QStandardItem("Start"));
    tableModel->setHorizontalHeaderItem(2,new QStandardItem("Finish"));
    tableModel->setHorizontalHeaderItem(3,new QStandardItem("Length (periods)"));
    tableModel->setHorizontalHeaderItem(4,new QStandardItem("Confirmation"));


    unsigned int rowNum = 0;
    for(PatternMatchList::const_iterator matchIter = currentPatternMatches_->begin();
        matchIter != currentPatternMatches_->end(); matchIter++)
    {
        unsigned int colNum = 0;

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString((*matchIter)->matchType().c_str())));
        colNum++;

        QDateTime startTime = QDateHelper::boostToQDateTime((*matchIter)->startTime());
        QString dateFormat = "yyyy-MM-dd";
        tableModel->setItem(rowNum,colNum,new QStandardItem(startTime.toString(dateFormat)));
        colNum++;

        if((*matchIter)->isConfirmedMatch())
        {
            QDateTime endTime = QDateHelper::boostToQDateTime((*matchIter)->endTime());
            tableModel->setItem(rowNum,colNum,new QStandardItem(endTime.toString(dateFormat)));

        }
        else
        {
            tableModel->setItem(rowNum,colNum,new QStandardItem("Still Forming"));
        }
        colNum++;

        tableModel->setItem(rowNum,colNum,new QStandardItem(QString().sprintf("%d",(*matchIter)->numPeriods())));
        colNum++;

        if((*matchIter)->isConfirmedMatch())
        {
            tableModel->setItem(rowNum,colNum,new QStandardItem(QString().sprintf("%.2f",(*matchIter)->confirmationPrice())));
         }
        else
        {
            tableModel->setItem(rowNum,colNum,new QStandardItem("Still Forming"));
        }
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


    if(currentPatternMatches_->size() > 0)
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

    PatternMatchListPtr selectedPatterns = PatternMatchListPtr(new PatternMatchList());

    QModelIndexList patternSelectionList = selectionModel()->selectedRows();
    for(QModelIndexList::iterator selectionIter = patternSelectionList.begin();
        selectionIter != patternSelectionList.end(); selectionIter++)
    {
        unsigned int selectionRow = (*selectionIter).row();
        assert(selectionRow <= currentPatternMatches_->size());
        PatternMatchList::iterator matchListIter = currentPatternMatches_->begin();
        std::advance(matchListIter,selectionRow);
        PatternMatchPtr currMatch = (*matchListIter);
        selectedPatterns->push_back(currMatch);
    }

    emit patternMatchesSelected(selectedPatterns);
}

