#ifndef BOTTOMSELECTIONPANE_H
#define BOTTOMSELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>

namespace Ui {
class BottomSelectionPane;
}

class BottomSelectionPane : public QWidget
{
    Q_OBJECT

public:
    explicit BottomSelectionPane(QWidget *parent = 0);
    ~BottomSelectionPane();

private:
    Ui::BottomSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // BOTTOMSELECTIONPANE_H
