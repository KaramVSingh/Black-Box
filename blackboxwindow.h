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
#include "gates/dflipflop.h"
#include "gates/input.h"
#include "gates/output.h"
#include "gates/decoder.h"
#include "gates/encoder.h"
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
    struct InputConnection {
        Gate* internalGate, *externalGate;
        int internalIndex, externalIndex;
    };

    struct OutputConnection {
        Gate* internalGate;
        QVector<Gate*> externalGates;
        QVector<int> externalIndexes;
        int internalIndex;
    };

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    bool isValid(QString str);
    QString getText();
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
    QPoint mouse;
    int selected = -1;
    bool changingSelected = false;
    QImage moveIcon;
    QImage zoomInIcon;
    QImage zoomOutIcon;

    QString toBinary(int value, int digits);
    QVector<DFlipFlop*> dFlipFlops;
    QVector<int> dFlipFlopStates;
    QVector<CustomGate*> customGates;
    QVector<int> customGatesStates;
    QVector<Input*> testInputs;
    QVector<InputConnection> detatchedInputs;
    QVector<Gate::Connection> checkedInputs;
    QVector<QString> inputNames;
    QVector<Output*> testOutputs;
    QVector<OutputConnection> detatchedOutputs;
    QVector<QString> outputNames;
    QVector<int> numberOfBitsPerInput;
    QVector<int> numberOfBitsPerOutput;
    QVector<Gate*> inputTranslaters;
    QVector<Gate*> outputTranslaters;
    QVector<int> clockPorts;


};

#endif // BLACKBOXWINDOW_H
