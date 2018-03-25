#ifndef TOPSELECTIONPANE_H
#define TOPSELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>
#include <QDebug>

#include "detail.h"
#include "field.h"

namespace Ui {
class TopSelectionPane;
}

class TopSelectionPane : public QWidget
{
    Q_OBJECT

public:
    explicit TopSelectionPane(QWidget *parent = 0);
    ~TopSelectionPane();
    void init(Field *widget);

public slots:
    void clearSelected(Mode m, QString s);

private slots:

signals:
    void toolChanged(Mode newTool, QString data);

private:
    Ui::TopSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    QPoint mouse;
    bool hasMouse = false;
    int selected = -1;
    QImage moveIcon;
    QImage zoomInIcon;
    QImage zoomOutIcon;

};

#endif // TOPSELECTIONPANE_H
