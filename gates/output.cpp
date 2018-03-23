#include "output.h"

void Output::setMultiBit(bool isMulti)
{
    if(isMulti) {
        multiBitInputs.append(0);
    }
}

int Output::execute(int index)
{
    if(takenInputs.size() != 1) {
        value = -1;
        return -1;
    }

    value = inputs[0].gate->execute(inputs[0].otherIndex);
    return value;
}

bool Output::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "OUTPUT Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "OUTPUT Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    return true;
}

bool Output::addOutput(Gate *newGate, int thisIndex, int otherIndex)
{
    qDebug() << "adding output to OUTPUT gate is illegal";
    return false;
}

void Output::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Output::toImage(float zoom)
{
    QImage image(80, 32, QImage::Format_ARGB32);
    image.fill(qRgba(0, 0, 0, 0));
    QPainter paint;
    QPen pen(Qt::black);
    pen.setWidth(2);
    paint.begin(&image);

    if(value == -1) {
        pen.setColor(Qt::darkRed);
    }

    paint.setPen(pen);

    QFont font("default");
    font.setPointSize(9);
    paint.setFont(font);

    QString valueString = QString::number(value);

    paint.drawLine(QPoint(0, GRID_DENSITY), QPoint(GRID_DENSITY, 0));
    paint.drawLine(QPoint(0, GRID_DENSITY), QPoint(GRID_DENSITY, 2 * GRID_DENSITY));
    paint.drawText(GRID_DENSITY, GRID_DENSITY + GRID_DENSITY / 2 - 1, valueString);
    paint.end();

    image = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return image;
}

QVector<QPoint> Output::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    inputLocations.append(location + QPoint(0, GRID_DENSITY));
    return inputLocations;
}

QVector<QPoint> Output::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    return outputLocations;
}

GateType Output::toType()
{
    return GateType::OUTPUT;
}

QString Output::toString()
{
    return "OUTPUT";
}
