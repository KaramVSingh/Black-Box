#ifndef CLOCK_H
#define CLOCK_H

#include "gate.h"

class Clock: public Gate
{
public:
    Clock(QPoint l): Gate(l) { numberOfInputLines = 0; inputs.resize(numberOfInputLines); numberOfOutputLines = 1; outputs.resize(numberOfOutputLines); width = 32; length = 32; }
    int execute(int index);
    bool addInput(Gate* newGate, int thisIndex, int otherIndex);
    bool addOutput(Gate* newGate, int thisIndex, int otherIndex);
    void changeLocation(QPoint newLocation);
    QImage toImage(float zoom);
    QVector<QPoint> getInputLocations();
    QVector<QPoint> getOutputLocations();
    void changeValue();
    GateType toType();
    QString toString();
    Gate* removeInput(int index);
    void removeOutput(int index);

    int value = 0;
};

#endif // CLOCK_H
