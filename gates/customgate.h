#ifndef CUSTOMGATE_H
#define CUSTOMGATE_H

#include "gate.h"
#include "and.h"
#include "decoder.h"
#include "dflipflop.h"
#include "encoder.h"
#include "input.h"
#include "not.h"
#include "or.h"
#include "output.h"
#include "clock.h"

class CustomGate: public Gate
{
public:
    CustomGate(QPoint l): Gate(l) { numberOfInputLines = 0; inputs.resize(numberOfInputLines); numberOfOutputLines = 0; outputs.resize(numberOfOutputLines); width = 0; length = 0; }
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
    bool build(QString fileName);
    int numberOfStates = 0;

    void setState(int state);
    void update();
    void change();
    int state = 0;
    int nextState = 0;
    QVector<int> clockPorts;
    QVector<int> clkVals;
    QVector<int> inputPortBits;
    QVector<int> outputPortBits;
    QVector<int> triggeredPorts;


private:
    int referenceIndex;
    int lengthOfRow;
    QImage gateImage;
    QString gateName;
    int getInt(QString content, int* index);
    QString getWord(QString content, int* index);

};

#endif // CUSTOMGATE_H
