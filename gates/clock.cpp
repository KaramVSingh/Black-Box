#include "clock.h"

void Clock::changeValue()
{
    value = (1 - value) * -1;
}

int Clock::execute(int index)
{
    return value;
}

bool Clock::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    qDebug() << "adding input to CLOCK gate is illegal";
    return false;
}

bool Clock::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "Clock Gate adding to illegal output index";
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

void Clock::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Clock::toImage(float zoom)
{
    QImage image(32, 32, QImage::Format_ARGB32);
    image.fill(qRgba(0, 0, 0, 0));
    QPainter paint;
    QPen pen(Qt::black);
    pen.setWidth(2);
    paint.begin(&image);
    paint.setPen(pen);

    QFont font("default");
    font.setPointSize(9);
    QFontMetrics metrics(font);
    paint.setFont(font);
    QString valueString = QString::number(value);
    int widthOfString = metrics.width(valueString);
    int heightOfString = metrics.height();

    paint.drawRect(4, 4, 24, 24);
    paint.drawLine(QPoint(28, 16), QPoint(32, 16));
    paint.drawText(QPoint(16 - widthOfString / 2, 12 + heightOfString / 2), QString::number(value));

    paint.end();

    image = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return image;
}

QVector<QPoint> Clock::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    return inputLocations;
}

QVector<QPoint> Clock::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(location + QPoint((int)(2 * GRID_DENSITY), (int)(GRID_DENSITY)));
    return outputLocations;
}

GateType Clock::toType()
{
    return GateType::CLOCK;
}

QString Clock::toString()
{
    return "CLOCK";
}
