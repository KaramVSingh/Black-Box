#ifndef AND_H
#define AND_H

#include "gate.h"

class And: public Gate
{
public:
    And(QPoint l): Gate(l) { numberOfInputLines = 2; inputs.resize(numberOfInputLines); numberOfOutputLines = 1; outputs.resize(numberOfOutputLines); width = 64; length = 64; }
    int execute(int index);
    bool addInput(Gate* newGate, int thisIndex, int otherIndex);
    bool addOutput(Gate* newGate, int thisIndex, int otherIndex);
    void changeLocation(QPoint newLocation);
    QImage toImage(float zoom);
    QVector<QPoint> getInputLocations(float zoom);
    QVector<QPoint> getOutputLocations(float zoom);
    void changeValue() {}
    gateType toType();
};

#endif // AND_H
