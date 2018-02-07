#ifndef TOPSELECTIONPANE_H
#define TOPSELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>

namespace Ui {
class TopSelectionPane;
}

class TopSelectionPane : public QWidget
{
    Q_OBJECT

public:
    explicit TopSelectionPane(QWidget *parent = 0);
    ~TopSelectionPane();

private:
    Ui::TopSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // TOPSELECTIONPANE_H
