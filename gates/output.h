#ifndef OUTPUT_H
#define OUTPUT_H

#include "gate.h"

class Output:public Gate
{
public:
    Output(QPoint l): Gate(l) { numberOfInputLines = 1; inputs.resize(numberOfInputLines); numberOfOutputLines = 0; outputs.resize(numberOfOutputLines); width = 80; length = 32; }
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

    int value = -1;
};

#endif // OUTPUT_H
