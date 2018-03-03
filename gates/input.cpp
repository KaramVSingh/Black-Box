#include "input.h"

void Input::changeValue()
{
    value = -1 * (value - 1);
}

int Input::execute(int index)
{
    return value;
}

bool Input::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    qDebug() << "adding input to INPUT gate is illegal";
    return false;
}

bool Input::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "INPUT Gate adding to illegal output index";
        return false;
    }

    // we dont care if taken outputs containts this becasue a single gate can have multiple
    // gates connected to a single output port

    takenOutputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    outputs[thisIndex].append(newConnection);

    return true;
}

void Input::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage Input::toImage(float zoom)
{
    QImage image(80, 32, QImage::Format_ARGB32);
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

    if(!multiBit) {
        paint.drawLine(QPoint(5 * GRID_DENSITY, GRID_DENSITY), QPoint(4 * GRID_DENSITY, 0));
        paint.drawLine(QPoint(5 * GRID_DENSITY, GRID_DENSITY), QPoint(4 * GRID_DENSITY, 2 * GRID_DENSITY));
        paint.drawText(4 * GRID_DENSITY - widthOfString, GRID_DENSITY + heightOfString / 4 + 1, valueString);
    } else {
        paint.drawLine(QPoint(5 * GRID_DENSITY, GRID_DENSITY), QPoint(4 * GRID_DENSITY, 0));
        paint.drawLine(QPoint(5 * GRID_DENSITY, GRID_DENSITY), QPoint(4 * GRID_DENSITY, 2 * GRID_DENSITY));
        paint.drawLine(QPoint(5 * GRID_DENSITY - GRID_DENSITY / 3, GRID_DENSITY), QPoint(4 * GRID_DENSITY - GRID_DENSITY / 3, 0));
        paint.drawLine(QPoint(5 * GRID_DENSITY - GRID_DENSITY / 3, GRID_DENSITY), QPoint(4 * GRID_DENSITY - GRID_DENSITY / 3, 2 * GRID_DENSITY));
        paint.drawText(4 * GRID_DENSITY - GRID_DENSITY / 3 - widthOfString, GRID_DENSITY + heightOfString / 4 + 1, valueString);
    }

    paint.end();

    image = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return image;
}

QVector<QPoint> Input::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    return inputLocations;
}

QVector<QPoint> Input::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(location + QPoint((int)(2 * GRID_DENSITY), (int)(GRID_DENSITY)));
    return outputLocations;
}

GateType Input::toType()
{
    return GateType::INPUT;
}

QString Input::toString()
{
    return "INPUT" + value;
}

void Input::setMultiBit(bool flag)
{
    multiBit = flag;
    if(multiBit) {
        multiBitOutputs.append(0);
    }
}
