#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plot.h"
#include <QItemSelectionModel>

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

};

#endif // MAINWINDOW_H
