#ifndef GATE_H
#define GATE_H

#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <QFileInfo>
#include <QDebug>

#include "detail.h"

class Gate
{
public:
    Gate(QPoint l) { location = l; }
    ~Gate() {}
    virtual QImage toImage(float zoom) {}
    virtual void changeLocation(QPoint newLocation) {}
    virtual int execute(int index) {}
    virtual bool addInput(Gate* newGate, int thisIndex, int otherIndex) {}
    virtual bool addOutput(Gate* newGate, int thisIndex, int otherIndex) {}
    virtual QVector<QPoint> getInputLocations() {}
    virtual QVector<QPoint> getOutputLocations() {}
    virtual void changeValue() {}
    virtual GateType toType() {}

    // this struct will be used to describe a connection to a gate's index
    struct Connection {
        Gate* gate;
        int otherIndex;
    };

    // this QVector will store all of the input gates and the input they are connected to (otherIndexes)
    QVector<Connection> inputs;
    int numberOfInputLines;

    // this QVector will store all of the output gates and the outputs they are connected to (otherIndexes)
    QVector<Connection> outputs;
    int numberOfOutputLines;

    QVector<int> takenInputs;
    QVector<int> takenOutputs;

    QPoint location;
    int width, length;
};

#endif // GATE_H
