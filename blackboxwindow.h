#ifndef BLACKBOXWINDOW_H
#define BLACKBOXWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QEventLoop>
#include <QRect>

#include "gates/gate.h"
#include "gates/customgate.h"
#include "wire.h"
#include "detail.h"

namespace Ui {
class BlackBoxWindow;
}

class BlackBoxWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BlackBoxWindow(QVector<Gate*> gates, QVector<Wire*> wires, QWidget *parent = 0);
    ~BlackBoxWindow();
    QString execute();

private:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    bool isValid(QString str);
    QString getText();
    CustomGate* getAssociatedBlackBox(Gate* gate);
    QString focusAndGetText(Gate* gate, int index, bool isInput);
    QPoint getFieldLocation(QPoint);
    QPoint startingPoint;
    Ui::BlackBoxWindow *ui;
    QVector<Gate*> gates;
    QVector<Wire*> wires;
    QPoint topLeftLocation;
    float zoom = 1;
    QPoint focusPoint;
    bool showRect = false;
    QVector<QVector<Gate*>> blackBoxGates;
    QPoint mouse;
    int selected = -1;
    bool changingSelected = false;
    QImage moveIcon;
    QImage zoomInIcon;
    QImage zoomOutIcon;

};

#endif // BLACKBOXWINDOW_H
