#ifndef INSTRUMENTLISTTABLEVIEW_H
#define INSTRUMENTLISTTABLEVIEW_H

#include <QTableView>
#include <QDir>
#include "InstrumentSelectionInfo.h"

class InstrumentListTableView : public QTableView
{
    Q_OBJECT

private:
    InstrumentSelectionInfoVector instrumentInfo_;
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
