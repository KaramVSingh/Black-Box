#include "or.h"

int Or::execute(int index)
{
    if(takenInputs.size() != 2) {
        return -1;
    }

    bool allZero = true;

    for(int i = 0; i < inputs.size(); i++) {
        int inputVal = inputs[i].gate->execute(inputs[i].otherIndex);
        if(inputVal == 1) {
            allZero = false;
        }

        if(inputVal == -1) {
            return -1;
        }
    }

    if(!allZero) {
        return 1;
    }

    return 0;
}

bool Or::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "OR Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "OR Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    return true;
}

bool Or::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "OR Gate adding to illegal output index";
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

void Or::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Or::toImage(float zoom)
{
    QString dir(QFileInfo(".").absolutePath());
    QPixmap pix(dir.append("/BlackBox/imageSources/items/or_gate.png"));
    QImage image = pix.toImage();
    QImage newImage = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return newImage;
}

QVector<QPoint> Or::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    inputLocations.append(location + QPoint(0, (int)(GRID_DENSITY)));
    inputLocations.append(location + QPoint(0, (int)(3 * GRID_DENSITY)));
    return inputLocations;
}

QVector<QPoint> Or::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(location + QPoint((int)(4 * GRID_DENSITY), (int)(2 * GRID_DENSITY)));
    return outputLocations;
}

GateType Or::toType()
{
    return GateType::OR;
}

QString Or::toString()
{
    return "OR";
}
