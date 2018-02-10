#include "wire.h"

Wire::Wire()
{
    moreThanOneVertex = false;
    hasIn = false;
    hasOut = false;
}

void Wire::addVertex(QPoint point)
{
    vertices.append(point);
}

void Wire::addInputConnection(Gate *gate, int gateIndex)
{
    input.gate = gate;
    input.otherIndex = gateIndex;
    hasIn = true;
}

void Wire::addOutputConnection(Gate *gate, int gateIndex)
{
    output.gate = gate;
    output.otherIndex = gateIndex;
    hasOut = true;
}

QVector<QPoint> Wire::getVertices()
{
    return vertices;
}

bool Wire::hasInput()
{
    return hasIn;
}

bool Wire::hasOutput()
{
    return hasOut;
}
