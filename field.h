#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QKeyEvent>

#include "detail.h"
#include "wire.h"

#include "gates/gate.h"
#include "gates/and.h"
#include "gates/not.h"
#include "gates/or.h"
#include "gates/input.h"
#include "gates/output.h"

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

signals:
    void keyTyped();

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
    void drawWire(QPoint point);
    void toggleInputs(QPoint point);
    void keyPressEvent(QKeyEvent *e);

    QVector<Gate*> gates;
    QVector<Input*> inputGates;
    QVector<Output*> outputGates;
    QVector<Wire*> wires;
    QPoint startLocation;
    float zoom;
    bool dragging;
    bool drawingWire;
    Mode tool;
    QString toolData;
    QString typedChar;


};

#endif // FIELD_H
