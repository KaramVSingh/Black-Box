#include "not.h"

int Not::execute(int index)
{
    if(takenInputs.size() != 1) {
        return -1;
    }

    int inputVal = inputs[0].gate->execute(inputs[0].otherIndex);

    if(inputVal == -1) {
        return -1;
    }

    return -1 * (inputVal - 1);
}

bool Not::addInput(Gate *newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "NOT Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "NOT Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    return true;
}

bool Not::addOutput(Gate *newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "NOT Gate adding to illegal output index";
        return false;
    }

    // we dont care if taken outputs containts this becasue a single gate can have multiple
    // gates connected to a single output port

    takenOutputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    outputs[thisIndex] = newConnection;
}

void Not::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Not::toImage(float zoom)
{
    QString dir(QFileInfo(".").absolutePath());
    QPixmap pix(dir.append("/guiV3/imageSources/items/not_gate.png"));
    QImage image = pix.toImage();
    QImage newImage = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return newImage;
}

QVector<QPoint> Not::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    inputLocations.append(location + QPoint(0, (int)(2 * GRID_DENSITY)));
    return inputLocations;
}

QVector<QPoint> Not::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(location + QPoint((int)(4 * GRID_DENSITY), (int)(2 * GRID_DENSITY)));
    return outputLocations;
}

GateType Not::toType()
{
    return GateType::NOT;
}

QString Not::toString()
{
    return "NOT";
}

