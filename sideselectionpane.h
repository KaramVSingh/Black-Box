#ifndef SIDESELECTIONPANE_H
#define SIDESELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>

namespace Ui {
class SideSelectionPane;
}

class SideSelectionPane : public QWidget
{
    Q_OBJECT

public:
    explicit SideSelectionPane(QWidget *parent = 0);
    ~SideSelectionPane();

private:
    Ui::SideSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // SIDESELECTIONPANE_H
