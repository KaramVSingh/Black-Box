#ifndef OR_H
#define OR_H

#include "gate.h"

class Or: public Gate
{
public:
    Or(QPoint l): Gate(l) { numberOfInputLines = 2; inputs.resize(numberOfInputLines); numberOfOutputLines = 1; outputs.resize(numberOfOutputLines); width = 64; length = 64; }
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

#endif // OR_H
