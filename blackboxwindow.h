#ifndef BLACKBOXWINDOW_H
#define BLACKBOXWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QEventLoop>

#include "gates/gate.h"
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

private slots:
    void on_moveButton_clicked();

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

private:

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    bool isValid(QString str);
    QString getText();
    QPoint getFieldLocation(QPoint);
    QPoint startingPoint;
    Ui::BlackBoxWindow *ui;
    QVector<Gate*> gates;
    QVector<Wire*> wires;
    QPoint topLeftLocation;
    float zoom = 1;
    Mode tool;

};

#endif // BLACKBOXWINDOW_H
