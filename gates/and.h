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
    QVector<QPoint> getInputLocations();
    QVector<QPoint> getOutputLocations();
    void changeValue() {}
    GateType toType();
    QString toString();
    Gate* removeInput(int index);
    void removeOutput(int index);
};

#endif // AND_H
