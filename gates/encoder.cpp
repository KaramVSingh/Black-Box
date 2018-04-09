#include "encoder.h"

int Encoder::execute(int index) {
    if(takenInputs.size() != numberOfInputLines) {
        return -1;
    }

    int value = 0;

    for(int i = 0; i < numberOfInputLines; i++) {
        int inputIndex = numberOfBits - i - 1;
        int inputValue = inputs[inputIndex].gate->execute(inputs[inputIndex].otherIndex);

        if(inputValue == -1) {
            return -1;
        }

        value += inputValue * (int)pow(2, i);
    }

    return value;
}

bool Encoder::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "ENCODER Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "ENCODER Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    return true;
}

bool Encoder::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "ENCODER Gate adding to illegal output index";
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

void Encoder::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Encoder::toImage(float zoom)
{
    QImage image(width, length, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter paint;
    QPen pen(Qt::black);
    pen.setWidth(3);
    paint.begin(&image);
    paint.setPen(pen);

    paint.drawLine(QPoint(GRID_DENSITY, 0), QPoint(width - 16 - GRID_DENSITY, 0));
    paint.drawLine(QPoint(width - 16 - GRID_DENSITY, length - 1), QPoint(GRID_DENSITY, length - 1));
    pen.setWidth(2);
    paint.setPen(pen);
    paint.drawLine(QPoint(width - 16 - GRID_DENSITY, 0), QPoint(width - GRID_DENSITY, 16));
    paint.drawLine(QPoint(width - GRID_DENSITY, 16), QPoint(width - GRID_DENSITY, length - 16));
    paint.drawLine(QPoint(width - GRID_DENSITY, length - 16), QPoint(width - 16 - GRID_DENSITY, length));
    paint.drawLine(QPoint(GRID_DENSITY, length), QPoint(GRID_DENSITY, 0));

    for(int i = 0; i < numberOfBits; i++) {
        paint.drawLine(QPoint(0, (i + 1) * GRID_DENSITY), QPoint(GRID_DENSITY, (i + 1) * GRID_DENSITY));
    }

    pen.setWidth(4);
    paint.setPen(pen);
    paint.drawLine(QPoint(width, GRID_DENSITY), QPoint(width - GRID_DENSITY + 1, GRID_DENSITY));

    paint.end();

    image = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return image;
}

QVector<QPoint> Encoder::getInputLocations()
{
    QVector<QPoint> inputLocations;
    for(int i = 0; i < numberOfBits; i++) {
        inputLocations.append(location + QPoint(0, (i + 1) * GRID_DENSITY));
    }
    return inputLocations;
}

QVector<QPoint> Encoder::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(location + QPoint(width, GRID_DENSITY));
    return outputLocations;
}

GateType Encoder::toType()
{
    return GateType::ENCODER;
}

QString Encoder::toString()
{
    return "ENCODER" + QString::number(numberOfBits);
}

void Encoder::setNumberOfBits(int numberOfBits)
{
    this->numberOfBits = numberOfBits;
    numberOfInputLines = numberOfBits;
    inputs.resize(numberOfBits);
    length = (numberOfBits + 1) * GRID_DENSITY;
}
