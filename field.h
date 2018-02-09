#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>

#include "detail.h"
#include "gate.h"
#include "and.h"

namespace Ui {
class Field;
}

class Field : public QWidget
{
    Q_OBJECT

public:
    explicit Field(QWidget *parent = 0);
    ~Field();
    QPoint topLeftLocation;

public slots:
    void changeTool(Mode newTool, QString data);

private:
    Ui::Field *ui;
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QPoint getFieldLocation(QPoint guiLocation);
    void placeGate(QPoint location);

    QVector<Gate*> gates;
    QPoint startLocation;
    float zoom;
    bool dragging;
    Mode tool;
    QString toolData;


};

#endif // FIELD_H
