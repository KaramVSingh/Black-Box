#include "customgate.h"

bool CustomGate::build(QString fileName)
{
    QString dir(QFileInfo(".").absolutePath());
    dir.append("/BlackBox/CustomGates/");
    dir.append(fileName);

    QFile file(dir);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return false;

    QString content = file.readAll();

    file.close();
    gateName = fileName.remove(".bb");
    int index = 0;

    // first thing we need to do is create all of the gates:
    int numberOfGates;
    // GATES = <number>:
    index += 8;

    numberOfGates = getInt(content, &index);

    // now go to next line
    index += 2;

    for(int i = 0; i < numberOfGates; i++) {
        getInt(content, &index);
        index += 2;
        QString gateName = getWord(content, &index);
        if(gateName == "AND") {
            And* g = new And(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            internalGates.append(g);
        } else if(gateName == "DFLIPFLOP") {
            DFlipFlop* g = new DFlipFlop(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            dFlipFlops.append(g);
            internalGates.append(g);
        } else if(gateName == "NOT") {
            Not* g = new Not(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            internalGates.append(g);
        } else if(gateName == "OR") {
            Or* g = new Or(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            internalGates.append(g);
        } else if(gateName.contains("DECODER")) {
            Decoder* g = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            gateName.remove("DECODER");
            g->setNumberOfBits(gateName.toInt());
            internalGates.append(g);
        } else if(gateName.contains("ENCODER")) {
            Encoder* g = new Encoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            gateName.remove("ENCODER");
            g->setNumberOfBits(gateName.toInt());
            internalGates.append(g);
        } else if(gateName == "CLOCK") {
            Clock* g = new Clock(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            clocks.append(g);
            internalGates.append(g);
        } else if(gateName.contains("INPUT")) {
            Input* g = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            gateName.remove("INPUT");
            g->value = gateName.toInt();
            internalGates.append(g);
        } else if(gateName == "OUTPUT") {
            Output* g = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
            internalGates.append(g);
        }

        index++;
    }

    // now we want to handle the connections
    index += 15;
    while(content[index] != QChar('~')) {
        int gateOne, gateTwo;
        int gateOneNode, gateTwoNode;

        gateOne = getInt(content, &index);
        index++;
        gateOneNode = getInt(content, &index);
        index += 4;
        gateTwo = getInt(content, &index);
        index++;
        gateTwoNode = getInt(content, &index);

        internalGates[gateTwo]->addInput(internalGates[gateOne], gateTwoNode, gateOneNode);
        internalGates[gateOne]->addOutput(internalGates[gateTwo], gateOneNode, gateTwoNode);
        index++;
    }

    index += 11;
    // now we want to get the inputs
    int numberOfInputs = getInt(content, &index);
    numberOfInputLines = numberOfInputs;
    inputPointers.resize(numberOfInputs);
    inputs.resize(numberOfInputs);
    index += 2;

    QVector<QString> inputNames;

    for(int i = 0; i < numberOfInputs; i++) {
        getInt(content, &index);
        index += 2;
        inputNames.append(getWord(content, &index));
        // we have to determine if this a multi bit input based on the gate that it is connected to
        index++;

        while(content[index] != QChar('~')) {

            index += 4;

            int gateNumber = getInt(content, &index);
            index++;
            int gateIndex = getInt(content, &index);

            if(!multiBitInputs.contains(i)) {
                // check if the gate it is connected to is a multi bit
                if(internalGates[gateNumber]->toType() == GateType::DECODER) {
                    multiBitInputs.append(i);
                }
            }
            Gate::Connection input;
            input.gate = internalGates[gateNumber];
            input.otherIndex = gateIndex;

            inputPointers[i].append(input);
        }

        index += 2;
    }

    index += 12;
    // now we need to connect the outputs properly
    int numberOfOutputs = getInt(content, &index);
    numberOfOutputLines = numberOfOutputs;
    outputPointers.resize(numberOfOutputs);
    outputs.resize(numberOfOutputs);

    index += 2;

    QVector<QString> outputNames;

    for(int i = 0; i < numberOfOutputs; i++) {
        getInt(content, &index);
        index += 2;
        outputNames.append(getWord(content, &index));
        index += 5;
        int gateNumber = getInt(content, &index);
        index++;
        int gateIndex = getInt(content, &index);
        index++;

        if(!multiBitOutputs.contains(i)) {
            if(internalGates[gateNumber]->toType() == GateType::ENCODER) {
                multiBitOutputs.append(i);
            }
        }

        Gate::Connection output;
        output.gate = internalGates[gateNumber];
        output.otherIndex = gateIndex;

        outputPointers[i] = output;
    }

    // we have now officially set up the gate:
    // now the last thing we need to do is set up the image and then we will have a functional gate
    // in order to do that we start by getting the width and the length of the image

    int maxInputOutput = numberOfInputs > numberOfOutputs ? numberOfInputs : numberOfOutputs;
    length = (maxInputOutput + 1) * GRID_DENSITY;
    // width is determined by the length of the names of the inputs

    QFont font("default");
    font.setPointSize(9);
    QFontMetrics metrics(font);


    QFont titleFont("default");
    titleFont.setPointSize(9);
    QFontMetrics titleMetrics(titleFont);

    int longestInputName = 0;
    int longestOutputName = 0;

    foreach(QString name, inputNames) {
        if(metrics.width(name) > longestInputName) {
            longestInputName = metrics.width(name);
        }
    }

    foreach(QString name, outputNames) {
        if(metrics.width(name) > longestOutputName) {
            longestOutputName = metrics.width(name);
        }
    }

    width = (longestInputName + 4 + longestOutputName + 4 + 4 * GRID_DENSITY);
    int add = GRID_DENSITY - width % GRID_DENSITY;
    width += add;

    int correctedTitleSize = titleMetrics.width(gateName) + GRID_DENSITY - titleMetrics.width(gateName) % 16;

    width = width > correctedTitleSize ? width : correctedTitleSize;

    length += 2 * GRID_DENSITY;

    QImage image(width, length, QImage::Format_ARGB32);
    image.fill(qRgba(0, 0, 0, 0));
    QPainter paint;
    QPen pen(Qt::black);
    pen.setWidth(2);
    QPen fourPen(Qt::black);
    fourPen.setWidth(4);
    paint.begin(&image);
    paint.setPen(pen);

    paint.drawRect(GRID_DENSITY, 1 + 2 * GRID_DENSITY, width - 2 * GRID_DENSITY, length - 2 * GRID_DENSITY - 2);

    for(int i = 0; i < numberOfInputs; i++) {
        if(multiBitInputs.contains(i)) {
            paint.setPen(fourPen);
        } else {
            paint.setPen(pen);
        }
        paint.drawLine(0, GRID_DENSITY * (i + 1) + 2 * GRID_DENSITY, GRID_DENSITY - 1, GRID_DENSITY * (i + 1) + 2 * GRID_DENSITY);
    }

    paint.setPen(pen);

    for(int i = 0; i < numberOfOutputs; i++) {
        if(multiBitOutputs.contains(i)) {
            paint.setPen(fourPen);
        } else {
            paint.setPen(pen);
        }
        paint.drawLine(width - GRID_DENSITY + 1, GRID_DENSITY * (i + 1) + 2 * GRID_DENSITY, width, GRID_DENSITY * (i + 1) + 2 * GRID_DENSITY);
    }

    paint.setPen(pen);

    // now we need to add the gate name and the port names
    paint.setFont(titleFont);
    paint.drawText(0, 2 * GRID_DENSITY - 4, gateName);

    paint.setFont(font);
    for(int i = 0; i < inputNames.size(); i++) {
        paint.drawText(GRID_DENSITY + 4, 2 * GRID_DENSITY + (i + 1) * GRID_DENSITY + 6, inputNames[i]);
    }

    for(int i = 0; i < outputNames.size(); i++) {
        paint.drawText(width - GRID_DENSITY - 4 - metrics.width(outputNames[i]), 2 * GRID_DENSITY + (i + 1) * GRID_DENSITY + 6, outputNames[i]);
    }


    gateImage = image;

    return true;
}

int CustomGate::getInt(QString content, int *index)
{
    QString sNumber;
    while(content[*index].isDigit()) {
        sNumber.append(content[*index]);
        (*index)++;
    }

    return sNumber.toInt();
}

QString CustomGate::getWord(QString content, int *index)
{
    QString word;
    while(content[*index] != QChar('\xa') && content[*index] != QChar('~')) {
        word.append(content[*index]);
        (*index)++;
    }

    return word;
}

QVector<DFlipFlop*> CustomGate::getAllDFlipFlops()
{
    return dFlipFlops;
}

QVector<Clock*> CustomGate::getAllClocks()
{
    return clocks;
}

int CustomGate::execute(int index)
{
    if(takenInputs.size() != numberOfInputLines) {
        return -1;
    }

    return outputPointers[index].gate->execute(outputPointers[index].otherIndex);
}

bool CustomGate::addInput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(takenInputs.contains(thisIndex)) {
        // in this case the new gate is being connected to an existing input
        qDebug() << "CUSTOM Gate adding to taken input index";
        return false;
    }

    if(thisIndex >= numberOfInputLines) {
        // this means its an invlaid index
        qDebug() << "CUSTOM Gate adding to illegal input index";
        return false;
    }

    takenInputs.append(thisIndex);
    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    inputs[thisIndex] = newConnection;

    if(newGate->toType() == GateType::CUSTOM) {
        CustomGate* temp = static_cast<CustomGate*>(newGate);
        for(int i = 0; i < inputPointers[thisIndex].size(); i++) {
            Gate::Connection newCon;
            newCon.gate = inputPointers[thisIndex][i].gate;
            newCon.otherIndex = inputPointers[thisIndex][i].otherIndex;
            temp->outputPointers[otherIndex].gate->addOutput(newCon.gate, temp->outputPointers[otherIndex].otherIndex, newCon.otherIndex);
        }

        foreach(Gate::Connection connection, inputPointers[thisIndex]) {
            connection.gate->addInput(temp->outputPointers[otherIndex].gate, connection.otherIndex, temp->outputPointers[otherIndex].otherIndex);
        }

    } else {
        // this goes into new gate and connects in to the cutom gate
        for(int i = 0; i < inputPointers[thisIndex].size(); i++) {
            Gate::Connection newCon;
            newCon.gate = inputPointers[thisIndex][i].gate;
            newCon.otherIndex = inputPointers[thisIndex][i].otherIndex;
            newGate->outputs[otherIndex].append(newCon);
        }

        foreach(Gate::Connection connection, inputPointers[thisIndex]) {
            connection.gate->addInput(newGate, connection.otherIndex, otherIndex);
        }

    }

    return true;
}

bool CustomGate::addOutput(Gate* newGate, int thisIndex, int otherIndex)
{
    if(thisIndex >= numberOfOutputLines) {
        // this means its an invalid index
        qDebug() << "CUSTOM Gate adding to illegal output index";
        return false;
    }

    // we dont care if taken outputs containts this becasue a single gate can have multiple
    // gates connected to a single output port

    takenOutputs.append(thisIndex);

    newGate->inputs[otherIndex].gate = outputPointers[thisIndex].gate;
    newGate->inputs[otherIndex].otherIndex = outputPointers[thisIndex].otherIndex;
    outputPointers[thisIndex].gate->addOutput(newGate, outputPointers[thisIndex].otherIndex, otherIndex);


    // we dont want to add the outputs of gates to an outputPointers list because it is unnesesary
    // and because we dont want 2 outputs going to one wire as a matter of semantics
    // we do want to connect the gate straight to the input so execute is neve called

    if(newGate->toType() == GateType::CUSTOM) {
        return true;
    }

    Connection newConnection;
    newConnection.gate = newGate;
    newConnection.otherIndex = otherIndex;
    outputs[thisIndex].append(newConnection);


    return true;
}

void CustomGate::changeLocation(QPoint newLocation)
{
    this->location = newLocation;
}

QImage CustomGate::toImage(float zoom)
{
    QImage newImage = gateImage.scaled((int)(width * zoom), (int)(length * zoom), Qt::KeepAspectRatio);
    return newImage;
}

QVector<QPoint> CustomGate::getInputLocations()
{
    // first input should be 16 points down and the second should be 48 (not accounting for zoom)
    QVector<QPoint> inputLocations;
    for(int i = 0; i < numberOfInputLines; i++) {
        inputLocations.append(location + QPoint(0, GRID_DENSITY + i * GRID_DENSITY + 2 * GRID_DENSITY));
    }

    return inputLocations;
}

QVector<QPoint> CustomGate::getOutputLocations()
{
    QVector<QPoint> outputLocations;
    for(int i = 0; i < numberOfOutputLines; i++) {
        outputLocations.append(location + QPoint(width , GRID_DENSITY + i * GRID_DENSITY + 2 * GRID_DENSITY));
    }
    return outputLocations;
}

GateType CustomGate::toType()
{
    return GateType::CUSTOM;
}

QString CustomGate::toString()
{
    return gateName;
}
