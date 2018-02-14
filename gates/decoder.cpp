#include "decoder.h"

int Decoder::execute(int index) {
    if(takenInputs.size() != 1) {
        return -1;
    }

    int input = inputs[0].gate->execute(inputs[0].otherIndex);

    if(input == -1) {
        return -1;
    }

    QVector<int> digits;
    while(input > 0) {
        digits.append(input % 2);
        input /= 2;
    }

    if(digits.size() < numberOfBits - index) {
        return 0;
    }

    int bitNumber = numberOfBits - 1 - index;
    return digits[bitNumber];
}

bool Decoder::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "DECODER Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "DECODER Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    return true;
}

bool Decoder::addOutput(Gate *newGate, int thisIndex, int otherIndex)
{
    if(takenOutputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "DECODER Gate adding to taken output index";
        return false;
    }

    if(thisIndex >= numberOfOutputLines) {
        // this means its an invlaid index
        qDebug() << "DECODER Gate adding to illegal output index";
        return false;
    }

    takenOutputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    outputs[thisIndex] = newConnection;

    return true;
}

void Decoder::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Decoder::toImage(float zoom)
{
    QImage image(width, length, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter paint;
    QPen pen(Qt::black);
    pen.setWidth(2);
    paint.begin(&image);
    paint.setPen(pen);

    paint.drawLine(QPoint(GRID_DENSITY, GRID_DENSITY), QPoint(width - GRID_DENSITY, 0));
    paint.drawLine(QPoint(GRID_DENSITY, GRID_DENSITY), QPoint(width - GRID_DENSITY, length));
    paint.drawLine(QPoint(width - GRID_DENSITY, length), QPoint(width - GRID_DENSITY, 0));
    paint.drawLine(QPoint(0, GRID_DENSITY), QPoint(GRID_DENSITY, GRID_DENSITY));

    for(int i = 0; i < numberOfBits; i++) {
        paint.drawLine(QPoint(width - GRID_DENSITY, (i + 1) * GRID_DENSITY), QPoint(width, (i + 1) * GRID_DENSITY));
    }

    paint.end();

    image = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return image;
}

QVector<QPoint> Decoder::getInputLocations()
{
    QVector<QPoint> inputLocations;
    inputLocations.append(location + QPoint(0, GRID_DENSITY));
    return inputLocations;
}

QVector<QPoint> Decoder::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    for(int i = 0; i < numberOfBits; i++) {
        outputLocations.append(location + QPoint(width, (i + 1) * GRID_DENSITY));
    }
    return outputLocations;
}

GateType Decoder::toType()
{
    return GateType::DECODER;
}

QString Decoder::toString()
{
    return "DECODER" + numberOfBits;
}

void Decoder::setNumberOfBits(int numberOfBits)
{
    this->numberOfBits = numberOfBits;
    numberOfOutputLines = numberOfBits;
    outputs.resize(numberOfBits);
    length = (numberOfBits + 1) * GRID_DENSITY;
}
