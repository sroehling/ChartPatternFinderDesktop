#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plot.h"
#include <QItemSelectionModel>
#include <QTableView>
#include "PatternMatch.h"
#include "PatternMatchTableView.h"
#include "InstrumentListTableView.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void instrumentSelected(const InstrumentSelectionInfoPtr &instrSelectionInfo);
    void patternMatchSelected(const PatternMatchPtr &selectedMatch);

private:
    Plot *d_plot;
    PatternMatchTableView* patternTable_;
    InstrumentListTableView *instrumentListTableView_;
};

#endif // MAINWINDOW_H
