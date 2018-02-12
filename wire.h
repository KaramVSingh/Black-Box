#ifndef WIRE_H
#define WIRE_H

#include "gates/gate.h"
#include "detail.h"

class Wire
{
public:
    Wire();
    QVector<QPoint> vertices;
    Gate::Connection input;
    Gate::Connection output;
    bool moreThanOneVertex;
    bool hasIn, hasOut;

    void addVertex(QPoint point);
    void addInputConnection(Gate* gate, int gateIndex);
    void addOutputConnection(Gate* gate, int gateIndex);
    QVector<QPoint> getVertices();
    bool hasInput();
    bool hasOutput();
};

#endif // WIRE_H
