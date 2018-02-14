#ifndef INPUT_H
#define INPUT_H

#include "gate.h"

class Input:public Gate
{
public:
    Input(QPoint l): Gate(l) { numberOfInputLines = 0; inputs.resize(numberOfInputLines); numberOfOutputLines = 1; outputs.resize(numberOfOutputLines); width = 80; length = 32; }
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
    void setMultiBit(bool flag);

    int value = 0;
    bool multiBit = false;
};

#endif // INPUT_H
