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

class CustomGate: public Gate
{
public:

    struct GateToGateConnection {
        int startGate, startIndex;
        int endGate, endIndex;
    };

    struct PointToGateConnection {
        QString name;
        int gateNumber, connectionIndex;
    };

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
    bool build(QString fileName);
    QVector<DFlipFlop*> getAllDFlipFlops();
    QVector<QString> rawGateNames;
    QVector<GateToGateConnection> rawConnections;
    QVector<QList<PointToGateConnection>> rawInputs;
    QVector<PointToGateConnection> rawOutputs;


private:
    QVector<Gate*> internalGates;
    QVector<QList<Gate::Connection>> inputPointers;
    QVector<Gate::Connection> outputPointers;
    QVector<DFlipFlop*> dFlipFlops;
    QImage gateImage;
    QString gateName;
    int getInt(QString content, int* index);
    QString getWord(QString content, int* index);

};

#endif // CUSTOMGATE_H
