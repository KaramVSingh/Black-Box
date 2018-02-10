#include "and.h"

// this function will call execute on all of the inputs and and them. index does not
// matter for this particular gate because it has one output
int And::execute(int index)
{
    if(takenInputs.size() != 2) {
        return -1;
    }

    for(int i = 0; i < inputs.size(); i++) {
        int inputVal = inputs[i].gate->execute(inputs[i].otherIndex);
        if(inputVal == 0) {
            return 0;
        }

        if(inputVal == -1) {
            return -1;
        }
    }

    return 1;
}

// this function returns false if unable to add the input and manages inputs
bool And::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "AND Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "AND Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;
}

// this function returns false if unable to add the output and manages the outputs
bool And::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "AND Gate adding to illegal output index";
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

// this function changes the location of the gate
void And::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

// this function returns an image that is properly zoomed
QImage And::toImage(float zoom)
{
    QString dir(QFileInfo(".").absolutePath());
    QPixmap pix(dir.append("/guiV3/imageSources/items/and_gate.png"));
    QImage image = pix.toImage();
    QImage newImage = image.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return newImage;
}

// this function returns the locations of the input locations properly zoomed
QVector<QPoint> And::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    inputLocations.append(location + QPoint(0, (int)(GRID_DENSITY)));
    inputLocations.append(location + QPoint(0, (int)(3 * GRID_DENSITY)));
    return inputLocations;
}

// this function returns the locations of the output locations properly zoomed
QVector<QPoint> And::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    outputLocations.append(location + QPoint((int)(4 * GRID_DENSITY), (int)(2 * GRID_DENSITY)));
    return outputLocations;
}

// this will be use to identify what type of gate this is in vectors of gates
GateType And::toType()
{
    return GateType::AND;
}
