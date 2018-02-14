#ifndef DECODER_H
#define DECODER_H

#include "gate.h"

class Decoder: public Gate
{
public:
    Decoder(QPoint l): Gate(l) { numberOfInputLines = 1; inputs.resize(numberOfInputLines); multiBitInputs.append(0); numberOfOutputLines = 1; outputs.resize(numberOfOutputLines); width = 64; length = 64; }
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

    int numberOfBits = 1;
    void setNumberOfBits(int numberOfBits);
};

#endif // DECODER_H
