#include "dflipflop.h"

void DFlipFlop::update()
{
    if(takenInputs.size() != 2) {
        nextState = -1;
        return;
    }

    int clk = inputs[1].gate->execute(inputs[1].otherIndex);

    if(clk == 1 && clkVal == 0) {
        // the clk is on a rising edge
        nextState = inputs[0].gate->execute(inputs[0].otherIndex);
        clkVal = 1;
        return;
    }

    clkVal = clk;

    nextState = state;
}

void DFlipFlop::change()
{
    state = nextState;
}

int DFlipFlop::execute(int index)
{
    if(index == 0) {
        return state;
    } else {
        if(state == -1) {
            return -1;
        }
        return (-1 * (state - 1));
    }
}

bool DFlipFlop::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "DFLIPFLOP Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "DFLIPFLOP Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    return true;
}

bool DFlipFlop::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "DFLIPFLOP Gate adding to illegal output index";
        return false;
    }

    // we dont care if taken outputs containts this becasue a single gate can have multiple
    // gates connected to a single output port

    takenOutputs.append(thisIndex);
    if(newGate->toType() == GateType::CUSTOM) {
        return true;
    }
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    outputs[thisIndex].append(newConnection);

    return true;
}

void DFlipFlop::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QVector<QPoint> DFlipFlop::getInputLocations()
{
    QVector<QPoint> inputLocations;
    inputLocations.append(QPoint(location.x(), location.y() + GRID_DENSITY));
    inputLocations.append(QPoint(location.x(), location.y() + 4 * GRID_DENSITY));

    return inputLocations;
}

QVector<QPoint> DFlipFlop::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(QPoint(location.x() + 5 * GRID_DENSITY, location.y() + GRID_DENSITY));
    outputLocations.append(QPoint(location.x() + 5 * GRID_DENSITY, location.y() + 4 * GRID_DENSITY));

    return outputLocations;
}

QImage DFlipFlop::toImage(float zoom)
{
    QString dir(QFileInfo(".").absolutePath());
    QPixmap pix(dir.append("/BlackBox/imageSources/items/dflipflop_gate.png"));
    QImage image = pix.toImage();
    QImage newImage = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return newImage;
}

GateType DFlipFlop::toType()
{
    return GateType::DFLIPFLOP;
}

QString DFlipFlop::toString()
{
    return "DFLIPFLOP";
}
