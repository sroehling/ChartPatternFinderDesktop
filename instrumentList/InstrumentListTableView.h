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
    void selectInstrument(int instrNum);

public:
    explicit InstrumentListTableView();
    void populateFromCSVFiles(QString quoteFilePath);

public slots:
    void instrumentSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  & );
    void instrumentAddedToList(unsigned int instrNum);
signals:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelInfo);

public slots:

};

#endif // INSTRUMENTLISTTABLEVIEW_H
