#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plot.h"
#include <QItemSelectionModel>
#include <QTableView>
#include "PatternMatch.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void patternTableCellSelected(QModelIndex &index);

public slots:
    void patternTableSelectionChanged (const QItemSelection  &selected,
                                          const QItemSelection  &deselected );

private:
    Plot *d_plot;
    QTableView* patternTable_;

    // For some reason, using the PatternMatchListPtr causes a crash.
    // TODO - Need to find the cause of this.
    PatternMatchList *currentPatternMatches_;

};

#endif // MAINWINDOW_H
