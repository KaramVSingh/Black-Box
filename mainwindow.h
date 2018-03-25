#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "topselectionpane.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *e);
    TopSelectionPane* pane;
};

#endif // MAINWINDOW_H
