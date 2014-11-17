#ifndef INSTRUMENTLISTTABLEVIEW_H
#define INSTRUMENTLISTTABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>
#include <QDir>
#include "InstrumentSelectionInfo.h"
#include "InstrumentList.h"

class InstrumentListTableView : public QTableView
{
    Q_OBJECT

private:
    InstrumentListPtr instrumentList_;

    QStandardItemModel *tableModel_;

    void initTable();
    void obsoleteCurrentInstrumentList();
    void selectInstrument(int instrNum);

public:
    explicit InstrumentListTableView();
    void populateFromCSVFiles(QString quoteFilePath);
    void appExitCleanupHandler();

public slots:
    void instrumentSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  & );
    void instrumentAddedToList(unsigned int instrNum);
    void instrListScanProgressUpdated(unsigned int numInstrsScanned);
    void instrListScanComplete();
signals:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelInfo);
    void startingPatternScan(unsigned int numInstruments);
    void instrumentScanProgressUpdated(unsigned int totalInstrumentsScanned);
    void instrumentScanComplete();

public slots:

};

#endif // INSTRUMENTLISTTABLEVIEW_H
