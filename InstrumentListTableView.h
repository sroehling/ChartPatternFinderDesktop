#ifndef INSTRUMENTLISTTABLEVIEW_H
#define INSTRUMENTLISTTABLEVIEW_H

#include <QTableView>
#include <QDir>
#include "InstrumentSelectionInfo.h"
#include "InstrumentList.h"

class InstrumentListTableView : public QTableView
{
    Q_OBJECT

private:
    InstrumentListPtr instrumentList_;

    void populateTable();
    void selectInstrument(int instrNum);

public:
    explicit InstrumentListTableView();
    void populateFromCSVFiles(QString quoteFilePath);

public slots:
    void instrumentSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  & );
signals:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelInfo);

public slots:

};

#endif // INSTRUMENTLISTTABLEVIEW_H
