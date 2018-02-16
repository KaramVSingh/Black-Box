#ifndef DFLIPFLOP_H
#define DFLIPFLOP_H

#include "gate.h"

class DFlipFlop: public Gate
{
public:
    DFlipFlop(QPoint l): Gate(l) { numberOfInputLines = 2; inputs.resize(numberOfInputLines); numberOfOutputLines = 2; outputs.resize(numberOfOutputLines); width = 80; length = 80; }
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

    int state = 0, nextState = 0;
    int clkVal = 0;
    void update();
    void change();
};

#endif // DFLIPFLOP_H
