#include "customgate.h"

void CustomGate::setState(int state)
{
    this->nextState = state;
    this->state = state;
}

void CustomGate::update()
{
    // this function is going to be very similar to that of the dflipflop

    // WE ARE INTENTIONALLY LEAVING A BUG HERE BECAUSE ITS TOO MUCH TO FIX RN. WILL ADRESS
    // WHEN THERE ARE LESS MOVING PARTS
    // if a clock port is a multibit input, then we have to focus on the right bits of the multi port
    if(takenInputs.size() != numberOfInputLines) {
        nextState = 0;
        return;
    }

    QVector<int> clks;
    for(int i = 0; i < clockPorts.size(); i++) {
        clks.append(inputs[clockPorts[i]].gate->execute(inputs[clockPorts[i]].otherIndex));
    }

    QVector<int> risingEdgePorts;
    for(int i = 0; i < clockPorts.size(); i++) {
        if(clks[i] == 1 && clkVals[i] == 0) {
            // this input is on the rising edge and is a clock input
            risingEdgePorts.append(clockPorts[i]);
        }
    }

    for(int i = 0; i < clockPorts.size(); i++) {
        clkVals[i] = clks[i];
    }

    if(risingEdgePorts.size() == 0) {
        // then we have to not change states
        nextState = state;
        return;
    }

    // we need to go to the state where the rising edge ports go back to 0
    triggeredPorts.clear();
    for(int i = 0; i < risingEdgePorts.size(); i++) {
        triggeredPorts.append(risingEdgePorts[i]);
    }

    QString dir(QFileInfo(".").absolutePath());
    dir.append("/BlackBox/CustomGates/");
    dir.append(gateName);
    dir.append(".bb");

    QFile file(dir);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString content = file.readAll();

    file.close();
    int contentIndex = referenceIndex;
    // first thing we need to do is go to the actual state
    contentIndex += state * lengthOfRow * pow(2, numberOfInputLines);

    QVector<int> results;
    for(int i = 0; i < numberOfInputLines; i++) {
        results.append(inputs[i].gate->execute(inputs[i].otherIndex));
    }

    QVector<int> refinedResult;
    for(int i = 0; i < numberOfInputLines; i++) {
        if(multiBitInputs.contains(i)) {
            for(int j = 0; j < inputPortBits[i]; j++) {
                refinedResult.append(results[i] % 2);
                results[i] = results[i] / 2;
            }
        } else {
            refinedResult.append(results[i]);
        }
    }

    int numberOfLines = 0;
    for(int i = 0; i < refinedResult.size(); i++) {
        numberOfLines += refinedResult[i] * pow(2, i);
    }

    contentIndex += numberOfLines * lengthOfRow;
    // this should put us on the right line
    // now all we need to do is look at the nextState
    int stateDigits = 0;
    if(floor(log2(numberOfStates)) == 0) {
        stateDigits = 1;
    } else {
        stateDigits = floor(log2(numberOfStates));
    }

    contentIndex += stateDigits + 1 + numberOfInputLines + 1;
    nextState = 0;
    for(int i = stateDigits - 1; i >= 0; i--) {
        nextState += content[contentIndex].digitValue() * pow(2, i);
        contentIndex++;
    }

    return;
}

void CustomGate::change()
{
    state = nextState;
}

Gate* CustomGate::removeInput(int index)
{
    Gate* save = inputs[index].gate;
    inputs[index].gate = NULL;
    takenInputs.remove(takenInputs.indexOf(index));
    return save;
}

void CustomGate::removeOutput(int index)
{
    for(int i = 0; i < outputs[index].size(); i++) {
        outputs[index].removeLast();
    }
    takenOutputs.remove(takenOutputs.indexOf(index));
}

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

    // we have now officially set up the gate:
    // now the last thing we need to do is set up the image and then we will have a functional gate
    // in order to do that we start by getting the width and the length of the image
    int numberOfInputs = getInt(content, &index);
    numberOfInputLines = numberOfInputs;
    inputs.resize(numberOfInputs);
    index++;
    int numberOfOutputs = getInt(content, &index);
    numberOfOutputLines = numberOfOutputs;
    outputs.resize(numberOfOutputs);
    index++;
    numberOfStates = getInt(content, &index);
    index++;

    // first we handle the inputs
    QVector<QString> inputNames;
    for(int i = 0; i < numberOfInputs; i++) {
        inputNames.append(getWord(content, &index));
        index++;
        if(content[index] == '1') {
            clockPorts.append(i);
        }
        index += 2;
        inputPortBits.append(getInt(content, &index));
        if(inputPortBits[i] > 1) {
            multiBitInputs.append(i);
        }
        index++;
    }

    // now we need to handle the outputs
    QVector<QString> outputNames;
    for(int i = 0; i < numberOfOutputs; i++) {
        outputNames.append(getWord(content, &index));
        index++;
        outputPortBits.append(getInt(content, &index));
        if(outputPortBits[i] > 1) {
            multiBitOutputs.append(i);
        }
        index++;
    }

    referenceIndex = index;
    int stateDigits = 0;
    if(floor(log2(numberOfStates)) == 0) {
        stateDigits = 1;
    } else {
        stateDigits = floor(log2(numberOfStates));
    }

    lengthOfRow = 4 + numberOfInputs + numberOfOutputs + 2 * stateDigits;
    index += lengthOfRow;

    for(int i = 0; i < clockPorts.size(); i++) {
        clkVals.append(0);
    }

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
    while(content[*index] != QChar('\xa') && content[*index] != QChar('%')) {
        word.append(content[*index]);
        (*index)++;
    }

    return word;
}

int CustomGate::execute(int index)
{
    QString dir(QFileInfo(".").absolutePath());
    dir.append("/BlackBox/CustomGates/");
    dir.append(gateName);
    dir.append(".bb");

    QFile file(dir);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString content = file.readAll();

    file.close();
    int contentIndex = referenceIndex;
    // first thing we need to do is go to the actual state
    contentIndex += state * lengthOfRow * pow(2, numberOfInputLines);

    // now we should be at the right state
    // now the next thing we need to do is check the values of all of the inputs to create
    // a string that represents the inputs
    QVector<int> results;
    for(int i = 0; i < numberOfInputLines; i++) {
        results.append(inputs[i].gate->execute(inputs[i].otherIndex));
    }

    QVector<int> refinedResult;
    for(int i = 0; i < numberOfInputLines; i++) {
        if(multiBitInputs.contains(i)) {
            for(int j = 0; j < inputPortBits[i]; j++) {
                refinedResult.append(results[i] % 2);
                results[i] = results[i] / 2;
            }
        } else {
            refinedResult.append(results[i]);
        }
    }

    // CHANGES WILL HAVE TO BE MADE HERE TOO
    // this presents another bug as well, if a clock input is triggered, it
    // internally sets that to 0. maybe thats not an issue but in specific scenarios
    // this will definitely create issues
    for(int i = 0; i < triggeredPorts.size(); i++) {
        refinedResult[triggeredPorts[i]] = 0;
    }

    int numberOfLines = 0;
    for(int i = 0; i < refinedResult.size(); i++) {
        numberOfLines += refinedResult[i] * pow(2, i);
    }

    contentIndex += numberOfLines * lengthOfRow;
    // this should put us on the right line
    // now all we need to do is look at the outputs
    int stateDigits = 0;
    if(floor(log2(numberOfStates)) == 0) {
        stateDigits = 1;
    } else {
        stateDigits = floor(log2(numberOfStates));
    }

    contentIndex += 2 * stateDigits + 3 + numberOfInputLines;
    // now we have the outputs.
    QVector<int> refinedOutput;
    for(int i = 0; i < numberOfOutputLines; i++) {
        if(multiBitOutputs.contains(i)) {
            refinedOutput.append(0);
            for(int j = 0; j < outputPortBits[i]; j++) {
                int bit = content[contentIndex].digitValue();
                refinedOutput[i] += bit * pow(2, j);
                contentIndex++;
            }
        } else {
            refinedOutput.append(content[contentIndex].digitValue());
            contentIndex++;
        }
    }

    return refinedOutput[refinedOutput.size() - 1 - index];
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
