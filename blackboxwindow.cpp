#include "blackboxwindow.h"
#include "ui_blackboxwindow.h"

BlackBoxWindow::BlackBoxWindow(QVector<Gate*> gates, QVector<Wire*> wires, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlackBoxWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    QPoint topLeftGate(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    foreach(Gate* g, gates) {
        if(g->location.x() < topLeftGate.x() && g->location.y() < topLeftGate.y()) {
            topLeftGate = g->location;
        }
    }

    topLeftGate -= QPoint(64, 64);
    topLeftLocation = topLeftGate;
    this->gates = gates;
    this->wires = wires;


    QString dir(QFileInfo(".").absolutePath());
    QPixmap pixMove(dir + "/BlackBox/imageSources/items/move_icon.png");
    moveIcon = pixMove.toImage();
    QPixmap pixZoomIn(dir + "/BlackBox/imageSources/items/zoom_icon.png");
    zoomInIcon = pixZoomIn.toImage();
    QPixmap pixZoomOut(dir + "/BlackBox/imageSources/items/zoomOut_icon.png");
    zoomOutIcon = pixZoomOut.toImage();

    update();
}

BlackBoxWindow::~BlackBoxWindow()
{
    delete ui;
}

void BlackBoxWindow::mousePressEvent(QMouseEvent *event)
{
    if(QRect(width() - 225, 75, 50, 50).contains(mouse)) {
        selected = 3;
        changingSelected = true;
        update();
        return;
    }

    if(QRect(width() - 150, 75, 50, 50).contains(mouse)) {
        selected = 2;
        changingSelected = true;
        update();
        return;
    }

    if(QRect(width() - 75, 75, 50, 50).contains(mouse)) {
        selected = 1;
        changingSelected = true;
        update();
        return;
    }

    startingPoint = getFieldLocation(QPoint(event->x(), event->y()));
    update();
    switch(selected) {
    case 2:
        if(zoom < 2) {
            zoom *= 2;
            // we also need to change the topLeftLocation. we want to have the width and
            // the height in terms of the new grid density. if we zoom in it is 32px
            int windowWidth = this->width() / GRID_DENSITY * GRID_DENSITY;
            int windowHeight = this ->height() / GRID_DENSITY * GRID_DENSITY;

            topLeftLocation.setX(startingPoint.x() - (int)(windowWidth / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
            topLeftLocation.setY(startingPoint.y() - (int)(windowHeight / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
        }
        break;
    case 1:
        if(zoom > 0.5) {
            zoom /= 2;
            // we also need to change the topLeftLocation
            int windowWidth = this->width() / GRID_DENSITY * GRID_DENSITY;
            int windowHeight = this ->height() / GRID_DENSITY * GRID_DENSITY;

            topLeftLocation.setX(startingPoint.x() - (int)(windowWidth / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
            topLeftLocation.setY(startingPoint.y() - (int)(windowHeight / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
        }
        break;
    }
}

void BlackBoxWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint fieldPoint = getFieldLocation(QPoint(event->x(), event->y()));

    if(changingSelected) {
        changingSelected = false;
        return;
    }

    switch(selected) {
    case 3:
        topLeftLocation += startingPoint - fieldPoint;
        break;
    }
    update();
}

void BlackBoxWindow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    QPen pen(Qt::black);
    pen.setWidth(2);
    paint.setPen(pen);

    paint.fillRect(0, 0, this->width(), this->height(), Qt::white);

    for(int i = 0; i < gates.size(); i++) {
        if(!gates[i]->render) {
            continue;
        }
        if(gates[i]->toType() != GateType::INPUT) {
            paint.drawImage((gates[i]->location - topLeftLocation) * zoom, gates[i]->toImage(zoom));
        } else {
            paint.drawImage((gates[i]->location - topLeftLocation - QPoint(3 * GRID_DENSITY, 0)) * zoom, gates[i]->toImage(zoom));
        }
    }

    for(int i = 0; i < wires.size(); i++) {

        if(!wires[i]->moreThanOneVertex) {
            continue;
        }

        if(wires[i]->multiBit) {
            QPen nPen(Qt::black);
            nPen.setWidth((int)(4 * zoom));
            paint.setPen(nPen);
        } else {
            QPen nPen(Qt::black);
            nPen.setWidth((int)(2 * zoom));
            paint.setPen(nPen);
        }

        for(int j = 0; j < wires[i]->vertices.size() - 1; j++) {
            paint.drawLine((wires[i]->vertices[j] - topLeftLocation) * zoom, (wires[i]->vertices[j + 1] - topLeftLocation) * zoom);
        }
    }

    paint.setPen(pen);

    if(showRect) {
        paint.drawRect((focusPoint.x() - 4 - topLeftLocation.x()) * zoom, (focusPoint.y() - 4 - topLeftLocation.y()) * zoom, 8 * zoom, 8 * zoom);
    }

    paint.fillRect(width() - 225, 75, 50, 50, QColor(39, 38, 67));
    paint.fillRect(width() - 150, 75, 50, 50, QColor(39, 38, 67));
    paint.fillRect(width() - 75, 75, 50, 50, QColor(39, 38, 67));

    if(QRect(width() - 225, 75, 50, 50).contains(mouse)) {
        paint.fillRect(width() - 225, 75, 50, 50, QColor(58, 78, 100));
    }

    if(QRect(width() - 150, 75, 50, 50).contains(mouse)) {
        paint.fillRect(width() - 150, 75, 50, 50, QColor(58, 78, 100));
    }

    if(QRect(width() - 75, 75, 50, 50).contains(mouse)) {
        paint.fillRect(width() - 75, 75, 50, 50, QColor(58, 78, 100));
    }

    if(selected != -1) {
        paint.fillRect(width() - selected * 75, 75, 50, 50, QColor(28, 187, 180));
    }

    paint.drawImage(width() - 225, 75, moveIcon);
    paint.drawImage(width() - 150, 75, zoomInIcon);
    paint.drawImage(width() - 75, 75, zoomOutIcon);
}

// used to translate between gui point and field point
QPoint BlackBoxWindow::getFieldLocation(QPoint guiLocation)
{
    // now we need to find the point that represents the location on the field
    QPoint fieldLocation(guiLocation);
    fieldLocation.setX((int)(fieldLocation.x() / zoom));
    fieldLocation.setY((int)(fieldLocation.y() / zoom));

    // we want to place it on the grid:
    if(fieldLocation.x() % (int)(GRID_DENSITY) < (int)(GRID_DENSITY / 2)) {
        fieldLocation.setX(fieldLocation.x() - fieldLocation.x() % (int)(GRID_DENSITY));
    } else {
        fieldLocation.setX((int)(fieldLocation.x() + (int)(GRID_DENSITY) - fieldLocation.x() % (int)(GRID_DENSITY)));
    }

    if(fieldLocation.y() % (int)(GRID_DENSITY) < (int)(GRID_DENSITY / 2)) {
        fieldLocation.setY(fieldLocation.y() - fieldLocation.y() % (int)(GRID_DENSITY));
    } else {
        fieldLocation.setY((int)(fieldLocation.y() + (int)(GRID_DENSITY) - fieldLocation.y() % (int)(GRID_DENSITY)));
    }

    fieldLocation += topLeftLocation;

    return fieldLocation;
}

QString BlackBoxWindow::execute()
{
    // will return the name of the gate
    // first thing we need to do is get the name of the new gate.
    // it cannot be a keyword so it can not be CONNECTIONS, GATES, INPUTS, or OUTPUTS
    QString gateName = getText();
    QString fileName = gateName;
    fileName = gateName + ".bb";

    QString gateDefinition;
    int numberOfInputs = 0, numberOfOutputs = 0, numberOfStates = 1;

    // first we want to count the inputs and outputs so we can get an idea of all of the states
    for(int i = 0; i < gates.size(); i++) {
        for(int j = 0; j < gates[i]->numberOfInputLines; j++) {
            // first the case where there is nothing connected to the input
            if(!gates[i]->takenInputs.contains(j)) {
                // if we are in this condition, we have a new input that is connected
                // to no other gates
                QString name = focusAndGetText(gates[i], j, true);
                if(name == "") {
                    continue;
                }
                inputNames.append(name);
                numberOfInputs++;
                // now we want to attatch an input which we can flip
                if(gates[i]->toType() == GateType::CUSTOM) {
                    // in this case we need to check if the input we are on is a multiBit
                    CustomGate* customGate = static_cast<CustomGate*>(gates[i]);
                    if(customGate->clockPorts.contains(j)) {
                        // that means we have a clock input that we need to propogate
                        clockPorts.append(numberOfInputs - 1);
                    }
                    Encoder* translater = new Encoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    translater->setNumberOfBits(customGate->inputPortBits[j]);
                    inputTranslaters.append(translater);
                    numberOfBitsPerInput.append(translater->numberOfBits);
                    translater->addOutput(gates[i], 0, j);
                    gates[i]->addInput(translater, j, 0);
                    for(int k = 0; k < translater->numberOfBits; k++) {
                        Input* newInput = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newInput->addOutput(translater, 0, k);
                        translater->addInput(newInput, k, 0);
                        testInputs.append(newInput);
                    }
                } else if(gates[i]->toType() == GateType::DECODER) {
                    // in this case we have a multibit and we need to make a multiBit
                    Encoder* translater = new Encoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    translater->setNumberOfBits(gates[i]->numberOfOutputLines);
                    inputTranslaters.append(translater);
                    numberOfBitsPerInput.append(translater->numberOfBits);
                    translater->addOutput(gates[i], 0, 0);
                    gates[i]->addInput(translater, 0, 0);
                    for(int k = 0; k < translater->numberOfBits; k++) {
                        Input* newInput = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newInput->addOutput(translater, 0, k);
                        translater->addInput(newInput, k, 0);
                        testInputs.append(newInput);
                    }
                } else {
                    if(gates[i]->toType() == GateType::DFLIPFLOP) {
                        if(j == 1) {
                            // this means that we are in the clock input port
                            clockPorts.append(numberOfInputs - 1);
                        }
                    }
                    Input* newInput = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    newInput->addOutput(gates[i], 0, j);
                    gates[i]->addInput(newInput, j, 0);
                    // we need to remember to recheck this later to detatch it
                    testInputs.append(newInput);
                    numberOfBitsPerInput.append(1);
                }
                continue;
            }

            // then the case in which there is something connected:
            // we need to make sure to check that there are no other shared connections.
            if(!gates.contains(gates[i]->inputs[j].gate)) {
                // if we have alread checked a gate connected to this gate through a
                // mutual input, we need to weed it out here:
                bool hasBeenChecked = false;
                for(int k = 0; k < checkedInputs.size(); k++) {
                    if(checkedInputs[k].gate == gates[i]->inputs[j].gate) {
                        if(checkedInputs[k].otherIndex == gates[i]->inputs[j].otherIndex) {
                            hasBeenChecked = true;
                            break;
                        }
                    }
                }

                if(hasBeenChecked) {
                    continue;
                }

                // this is the case where the connected gate is out of the selected area
                QString name = focusAndGetText(gates[i], j, true);
                if(name == "") {
                    continue;
                }
                inputNames.append(name);
                numberOfInputs++;
                int portIndex = gates[i]->inputs[j].otherIndex;
                Gate* inputGate = gates[i]->inputs[j].gate;
                QVector<Gate::Connection> connectedGates;
                for(int k = 0; k < gates[i]->inputs[j].gate->outputs[portIndex].size(); k++) {
                    connectedGates.append(gates[i]->inputs[j].gate->outputs[portIndex][k]);
                }

                for(int k = 0; k < connectedGates.size(); k++) {
                    // here we can check if the port is a clock port
                    if(connectedGates[k].gate->toType() == GateType::DFLIPFLOP) {
                        if(connectedGates[k].otherIndex == 1) {
                            clockPorts.append(numberOfInputs - 1);
                            break;
                        }
                    } else if(connectedGates[k].gate->toType() == GateType::CUSTOM) {
                        CustomGate* customGate = static_cast<CustomGate*>(connectedGates[k].gate);
                        if(customGate->clockPorts.contains(connectedGates[k].otherIndex)) {
                            clockPorts.append(numberOfInputs - 1);
                            break;
                        }
                    }
                }

                // now we need to handle multibit inputs
                if(gates[i]->toType() == GateType::CUSTOM) {
                    // in this case we need to check if the input we are on is a multiBit
                    CustomGate* customGate = static_cast<CustomGate*>(gates[i]);
                    Encoder* translater = new Encoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    translater->setNumberOfBits(customGate->inputPortBits[j]);
                    inputTranslaters.append(translater);
                    numberOfBitsPerInput.append(translater->numberOfBits);
                    for(int k = 0; k < translater->numberOfBits; k++) {
                        Input* newInput = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newInput->addOutput(translater, 0, k);
                        translater->addInput(newInput, k, 0);
                        testInputs.append(newInput);
                    }

                    // now we have set up the translater, we just have to connect the translater to
                    // all of the connected gates
                    for(int k = 0; k < connectedGates.size(); k++) {
                        Gate::Connection checkedInput;
                        checkedInput.gate = gates[i]->inputs[j].gate;
                        checkedInput.otherIndex = gates[i]->inputs[j].otherIndex;
                        checkedInputs.append(checkedInput);

                        InputConnection newConnection;
                        newConnection.externalGate = inputGate;
                        newConnection.externalIndex = portIndex;
                        newConnection.internalGate = connectedGates[k].gate;
                        newConnection.internalIndex = connectedGates[k].otherIndex;
                        detatchedInputs.append(newConnection);

                        // now we have to formally detatch the old input
                        connectedGates[k].gate->removeInput(connectedGates[k].otherIndex);

                        // and then we have to reattatch it all
                        connectedGates[k].gate->addInput(translater, connectedGates[k].otherIndex, 0);
                        translater->addOutput(connectedGates[k].gate, 0, connectedGates[k].otherIndex);
                    }
                } else if(gates[i]->toType() == GateType::DECODER) {
                    // in this case we have a multibit and we need to make a multiBit
                    Encoder* translater = new Encoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    translater->setNumberOfBits(gates[i]->numberOfOutputLines);
                    inputTranslaters.append(translater);
                    numberOfBitsPerInput.append(translater->numberOfBits);
                    for(int k = 0; k < translater->numberOfBits; k++) {
                        Input* newInput = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newInput->addOutput(translater, 0, k);
                        translater->addInput(newInput, k, 0);
                        testInputs.append(newInput);
                    }

                    // now we have set up the translater, we just have to connect the translater to
                    // all of the connected gates
                    for(int k = 0; k < connectedGates.size(); k++) {
                        Gate::Connection checkedInput;
                        checkedInput.gate = gates[i]->inputs[j].gate;
                        checkedInput.otherIndex = gates[i]->inputs[j].otherIndex;
                        checkedInputs.append(checkedInput);

                        InputConnection newConnection;
                        newConnection.externalGate = inputGate;
                        newConnection.externalIndex = portIndex;
                        newConnection.internalGate = connectedGates[k].gate;
                        newConnection.internalIndex = connectedGates[k].otherIndex;
                        detatchedInputs.append(newConnection);

                        // now we have to formally detatch the old input
                        connectedGates[k].gate->removeInput(connectedGates[k].otherIndex);

                        // and then we have to reattatch it all
                        connectedGates[k].gate->addInput(translater, connectedGates[k].otherIndex, 0);
                        translater->addOutput(connectedGates[k].gate, 0, connectedGates[k].otherIndex);
                    }
                } else {
                    Input* newInput = new Input(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    testInputs.append(newInput);
                    numberOfBitsPerInput.append(1);
                    for(int k = 0; k < connectedGates.size(); k++) {
                        // each index of outputs points to an index of the outputs.
                        // this is a list of all of the gates connected to that port
                        // we want to compile a list of all of the gates connected to that port.
                        // these gates must not be tested again in the future
                        Gate::Connection checkedInput;
                        checkedInput.gate = gates[i]->inputs[j].gate;
                        checkedInput.otherIndex = gates[i]->inputs[j].otherIndex;
                        checkedInputs.append(checkedInput);

                        InputConnection newConnection;
                        newConnection.externalGate = inputGate;
                        newConnection.externalIndex = portIndex;
                        newConnection.internalGate = connectedGates[k].gate;
                        newConnection.internalIndex = connectedGates[k].otherIndex;
                        detatchedInputs.append(newConnection);

                        // now we have to formally detatch the old input
                        connectedGates[k].gate->removeInput(connectedGates[k].otherIndex);

                        // and then proceed to reattatch it all
                        connectedGates[k].gate->addInput(newInput, connectedGates[k].otherIndex, 0);
                        newInput->addOutput(connectedGates[k].gate, 0, connectedGates[k].otherIndex);
                    }
                }
            }
        }
    }

    for(int i = 0; i < gates.size(); i++) {
        if(gates[i]->toType() == GateType::OUTPUT) {
            // if we are in this condition, we have an output which we deem
            // available to add to the list
            QString name = focusAndGetText(gates[i]->inputs[0].gate, gates[i]->inputs[0].otherIndex, false);
            if(name == "") {
                continue;
            }
            outputNames.append(name);
            numberOfOutputs++;
            int portIndex = gates[i]->inputs[0].otherIndex;

            OutputConnection newConnection;
            newConnection.internalGate = gates[i]->inputs[0].gate;
            newConnection.internalIndex = gates[i]->inputs[0].otherIndex;
            for(int k = 0; k < gates[i]->inputs[0].gate->outputs[portIndex].size(); k++) {
                newConnection.externalGates.append(gates[i]->inputs[0].gate->outputs[portIndex][k].gate);
                newConnection.externalIndexes.append(gates[i]->inputs[0].gate->outputs[portIndex][k].otherIndex);
            }
            detatchedOutputs.append(newConnection);

            int otherPort = gates[i]->inputs[0].otherIndex;
            gates[i]->inputs[0].gate->removeOutput(gates[i]->inputs[0].otherIndex);

            // now we can attach an output which we can trigger
            if(gates[i]->inputs[0].gate->toType() == GateType::CUSTOM) {
                CustomGate* customGate = static_cast<CustomGate*>(gates[i]->inputs[0].gate);
                Decoder* translater = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                translater->setNumberOfBits(customGate->outputPortBits[otherPort]);
                outputTranslaters.append(translater);
                numberOfBitsPerOutput.append(translater->numberOfBits);
                translater->addInput(gates[i]->inputs[0].gate, 0, otherPort);
                gates[i]->inputs[0].gate->addOutput(translater, otherPort, 0);
                for(int k = 0; k < translater->numberOfBits; k++) {
                    Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    newOutput->addInput(translater, 0, k);
                    translater->addOutput(newOutput, k, 0);
                    testOutputs.append(newOutput);
                }
            } else if(gates[i]->inputs[0].gate->toType() == GateType::ENCODER) {
                Decoder* translater = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                translater->setNumberOfBits(gates[i]->inputs[0].gate->numberOfInputLines);
                outputTranslaters.append(translater);
                translater->addInput(gates[i]->inputs[0].gate, 0, otherPort);
                gates[i]->inputs[0].gate->addOutput(translater, otherPort, 0);
                numberOfBitsPerOutput.append(translater->numberOfBits);
                for(int k = 0; k < translater->numberOfBits; k++) {
                    Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    newOutput->addInput(translater, 0, k);
                    translater->addOutput(newOutput, k, 0);
                    testOutputs.append(newOutput);
                }
            } else {
                Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                newOutput->addInput(gates[i]->inputs[0].gate, 0, otherPort);
                gates[i]->inputs[0].gate->addOutput(newOutput, otherPort, 0);
                // we need to remember to recheck this later to detach it
                testOutputs.append(newOutput);
                numberOfBitsPerOutput.append(1);
            }
            continue;
        }

        for(int j = 0; j < gates[i]->numberOfOutputLines; j++) {
            if(!gates[i]->takenOutputs.contains(j)) {
                // if we are in this condition, we have a new output that is connected
                // to no other gates
                QString name = focusAndGetText(gates[i], j, false);
                if(name == "") {
                    continue;
                }
                outputNames.append(name);
                numberOfOutputs++;
                // now we want to attatch an output which we can trigger
                if(gates[i]->toType() == GateType::CUSTOM) {
                    CustomGate* customGate = static_cast<CustomGate*>(gates[i]);
                    Decoder* translater = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    translater->setNumberOfBits(customGate->outputPortBits[j]);
                    outputTranslaters.append(translater);
                    translater->addInput(gates[i], 0, j);
                    gates[i]->addOutput(translater, j, 0);
                    numberOfBitsPerOutput.append(translater->numberOfBits);
                    for(int k = 0; k < translater->numberOfBits; k++) {
                        Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newOutput->addInput(translater, 0, k);
                        translater->addOutput(newOutput, k, 0);
                        testOutputs.append(newOutput);
                    }
                } else if(gates[i]->toType() == GateType::ENCODER) {
                    Decoder* translater = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    translater->setNumberOfBits(gates[i]->numberOfInputLines);
                    outputTranslaters.append(translater);
                    translater->addInput(gates[i], 0, 0);
                    gates[i]->addOutput(translater, 0, 0);
                    numberOfBitsPerOutput.append(translater->numberOfBits);
                    for(int k = 0; k < translater->numberOfBits; k++) {
                        Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newOutput->addInput(translater, 0, k);
                        translater->addOutput(newOutput, k, 0);
                        testOutputs.append(newOutput);
                    }
                } else {
                    Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                    newOutput->addInput(gates[i], 0, j);
                    gates[i]->addOutput(newOutput, j, 0);
                    // we need to remember to recheck this later to detatch it
                    testOutputs.append(newOutput);
                    numberOfBitsPerOutput.append(1);
                }
                continue;
            }

            // we need to check all of the gates that are connected to the output port to
            // see if the gate is an output
            for(int k = 0; k < gates[i]->outputs[j].size(); k++) {
                if(!gates.contains(gates[i]->outputs[j][k].gate)) {
                    // if we are in this condition, then we have a port whos output is outside of
                    // the selected area
                    QString name = focusAndGetText(gates[i], j, false);
                    if(name == "") {
                        continue;
                    }
                    outputNames.append(name);
                    numberOfOutputs++;

                    OutputConnection newConnection;
                    newConnection.internalGate = gates[i];
                    newConnection.internalIndex = j;
                    for(int l = 0; l < gates[i]->outputs[j].size(); l++) {
                        newConnection.externalGates.append(gates[i]->outputs[j][l].gate);
                        newConnection.externalIndexes.append(gates[i]->outputs[j][l].otherIndex);
                    }
                    detatchedOutputs.append(newConnection);

                    gates[i]->removeOutput(j);

                    // now we can attatch an output which we can trigger
                    if(gates[i]->toType() == GateType::CUSTOM) {
                        CustomGate* customGate = static_cast<CustomGate*>(gates[i]);
                        Decoder* translater = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        translater->setNumberOfBits(customGate->outputPortBits[j]);
                        outputTranslaters.append(translater);
                        translater->addInput(gates[i], 0, j);
                        gates[i]->addOutput(translater, j, 0);
                        numberOfBitsPerOutput.append(translater->numberOfBits);
                        for(int k = 0; k < translater->numberOfBits; k++) {
                            Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                            newOutput->addInput(translater, 0, k);
                            translater->addOutput(newOutput, k, 0);
                            testOutputs.append(newOutput);
                        }
                    } else if(gates[i]->toType() == GateType::ENCODER) {
                        Decoder* translater = new Decoder(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        translater->setNumberOfBits(gates[i]->numberOfInputLines);
                        outputTranslaters.append(translater);
                        translater->addInput(gates[i], 0, 0);
                        gates[i]->addOutput(translater, 0, 0);
                        numberOfBitsPerOutput.append(translater->numberOfBits);
                        for(int k = 0; k < translater->numberOfBits; k++) {
                            Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                            newOutput->addInput(translater, 0, k);
                            translater->addOutput(newOutput, k, 0);
                            testOutputs.append(newOutput);
                        }
                    } else {
                        Output* newOutput = new Output(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
                        newOutput->addInput(gates[i], 0, j);
                        gates[i]->addOutput(newOutput, j, 0);
                        // we need to remember to recheck this later to detatch it
                        testOutputs.append(newOutput);
                        numberOfBitsPerOutput.append(1);
                    }
                    break;
                }
            }
        }
    }

    // this piece of code will count the number of states that are in the system
    for(int i = 0; i < gates.size(); i++) {
        if(gates[i]->toType() == GateType::DFLIPFLOP) {
            numberOfStates *= 2;
        } else if(gates[i]->toType() == GateType::CUSTOM) {
            CustomGate* gate = static_cast<CustomGate*>(gates[i]);
            numberOfStates *= gate->numberOfStates;
        }
    }

    // now we are tasked with writing the number of states, inputs, outputs,
    // their names, and the number of bits per port. this will allow us to build the gate
    // first line will take the form # of inputs, # of outputs, # of states
    gateDefinition += QString::number(numberOfInputs) + ",";
    gateDefinition += QString::number(numberOfOutputs) + ",";
    gateDefinition += QString::number(numberOfStates) + "\r\n";

    // the next number of input lines will be formatted as follows:
    // <NAME>%<is clk>%<number of bits>
    for(int i = 0; i < numberOfInputs; i++) {
        gateDefinition += inputNames[i] + "%";
        if(clockPorts.contains(i)) {
            gateDefinition += "1%";
        } else {
            gateDefinition += "0%";
        }
        gateDefinition += QString::number(numberOfBitsPerInput[i]) + "\r\n";
    }

    // the next number of output lines will be formatted as follows:
    // <NAME>%<number of bits>
    for(int i = 0; i < numberOfOutputs; i++) {
        gateDefinition += outputNames[i] + "%";
        gateDefinition += QString::number(numberOfBitsPerOutput[i]) + "\r\n";
    }

    int stateDigits = 0;
    if(numberOfStates == 1) {
        stateDigits = 1;
    } else {
        stateDigits = floor(log2(numberOfStates));
    }

    for(int i = 0; i < gates.size(); i++) {
        if(gates[i]->toType() == GateType::DFLIPFLOP) {
            DFlipFlop* gate = static_cast<DFlipFlop*>(gates[i]);
            dFlipFlops.append(gate);
            dFlipFlopStates.append(gate->state);
        }

        if(gates[i]->toType() == GateType::CUSTOM) {
            CustomGate* gate = static_cast<CustomGate*>(gates[i]);
            customGates.append(gate);
            customGatesStates.append(gate->state);
        }
    }

    // now we have established the number of inputs, outputs, and states
    // we need to continue by creating the truth table
    for(int stateValue = 0; stateValue < numberOfStates; stateValue++) {
        for(int inputValue = 0; inputValue < pow(2, testInputs.size()); inputValue++) {
            gateDefinition += toBinary(stateValue, stateDigits) + "|";
            gateDefinition += toBinary(inputValue, testInputs.size()) + "|";
            // we need to set the state of the system, that means setting the state of all
            // of the custom gates and all of the dflipflops and all of the custom gates
            // we also need to set all of the inputs properly:

            for(int i = 0; i < customGates.size(); i++) {
                for(int j = 0; j < customGates[i]->clkVals.size(); j++) {
                    customGates[i]->clkVals[j] = 0;
                }
            }

            int tempInputValue = inputValue;
            for(int i = 0; i < testInputs.size(); i++) {
                testInputs[i]->setValue(tempInputValue % 2);
                tempInputValue /= 2;
            }

            int tempStateValue = stateValue;
            for(int i = 0; i < dFlipFlops.size(); i++) {
                dFlipFlops[i]->clkVal = 0;
                dFlipFlops[i]->setState(tempStateValue % 2);
                tempStateValue /= 2;
            }

            for(int i = 0; i < customGates.size(); i++) {
                int customState = 0;
                for(int j = 0; j < floor(log2(customGates[i]->numberOfStates)); j++) {
                    customState += (tempStateValue % 2) * pow(2, j);
                    tempStateValue /= 2;
                }
                customGates[i]->setState(customState);
            }

            // now the state has been set for the test run
            QVector<int> results;
            for(int i = 0; i < dFlipFlops.size(); i++) {
                for(int j = 0; j < dFlipFlops.size(); j++) {
                    dFlipFlops[j]->update();
                }

                for(int j = 0; j < dFlipFlops.size(); j++) {
                    dFlipFlops[j]->change();
                }
            }

            for(int i = 0; i < customGates.size(); i++) {
                for(int j = 0; j < customGates.size(); j++) {
                    customGates[j]->update();
                }

                for(int j = 0; j < customGates.size(); j++) {
                    customGates[j]->change();
                }
            }

            for(int i = 0; i < testOutputs.size(); i++) {
                results.append(testOutputs[testOutputs.size() - i - 1]->execute(0));
            }

            int newState = 0;
            int shift = 0;
            for(int i = 0; i < dFlipFlops.size(); i++) {
                newState += dFlipFlops[i]->state << shift;
                shift++;
            }

            for(int i = 0; i < customGates.size(); i++) {
                newState += customGates[i]->state << shift;
                shift += floor(log2(customGates[i]->numberOfStates));
            }


            if(floor(log2(numberOfStates)) == 0) {
                gateDefinition += "0|";
            } else {
                gateDefinition += toBinary(newState, floor(log2(numberOfStates))) + "|";
            }

            for(int i = 0; i < results.size(); i++) {
                gateDefinition += QString::number(results[i]);
            }

            gateDefinition += "\r\n";
        }
    }

    for(int i = 0; i < dFlipFlops.size(); i++) {
        dFlipFlops[i]->setState(dFlipFlopStates[i]);
    }

    for(int i = 0; i < customGates.size(); i++) {
        customGates[i]->setState(customGatesStates[i]);
    }

    // now we need to reset everything back to its origional setup
    // first we have to remove the outputTranslaters
    for(int i = 0; i < outputTranslaters.size(); i++) {
        int internalIndex = outputTranslaters[i]->inputs[0].otherIndex;
        outputTranslaters[i]->inputs[0].gate->removeOutput(internalIndex);
        outputTranslaters[i]->removeInput(0);
    }

    // then we need to remove the inputTranslaters
    for(int i = 0; i < inputTranslaters.size(); i++) {
        for(int j = 0; j < inputTranslaters[i]->outputs[0].size(); j++) {
            int internalIndex = inputTranslaters[i]->outputs[0][j].otherIndex;
            inputTranslaters[i]->outputs[0][j].gate->removeInput(internalIndex);
            inputTranslaters[i]->removeOutput(0);
        }
    }

    // now we have removed all of the translaters, now we want to disconnect the
    // input and output testers
    for(int i = 0; i < testOutputs.size(); i++) {
        int internalIndex = testOutputs[i]->inputs[0].otherIndex;
        testOutputs[i]->inputs[0].gate->removeOutput(internalIndex);
        testOutputs[i]->removeInput(0);
    }

    // now we need to remove the input testers
    for(int i = 0; i < testInputs.size(); i++) {
        for(int j = 0; j < testInputs[i]->outputs[0].size(); j++) {
            int internalIndex = testInputs[i]->outputs[0][j].otherIndex;
            testInputs[i]->outputs[0][j].gate->removeInput(internalIndex);
            testInputs[i]->removeOutput(0);
        }
    }

    // now we need to reattatch the old inputs and outputs
    // for that we have detatched inputs and detatched outputs
    // this should be a matter only with the qvector itself because it has all the info
    for(int i = 0; i < detatchedOutputs.size(); i++) {
        for(int j = 0; j < detatchedOutputs[i].externalGates.size(); j++) {
            detatchedOutputs[i].internalGate->addOutput(detatchedOutputs[i].externalGates[j], detatchedOutputs[i].internalIndex, detatchedOutputs[i].externalIndexes[j]);
        }
    }

    // same for detatched input
    for(int i = 0; i < detatchedInputs.size(); i++) {
        detatchedInputs[i].internalGate->addInput(detatchedInputs[i].externalGate, detatchedInputs[i].internalIndex, detatchedInputs[i].externalIndex);
    }

    QString dir(QFileInfo(".").absolutePath());
    dir.append("/BlackBox/CustomGates/");
    dir += fileName;

    QFile file(dir);
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);
    stream << gateDefinition;

    file.close();

    return gateName;
}

QString BlackBoxWindow::toBinary(int value, int digits)
{
    QVector<bool> bin;
    QString result = "";

    if(value == 0) {
        for(int i = 0; i < digits; i++) {
            result += "0";
        }

        return result;
    }

    for(int i = 0; i < digits - (floor(log2(value)) + 1); i++) {
        result += "0";
    }

    int storage = value;
    while(storage != 0) {
        if(storage % 2) {
            bin.append(true);
        } else {
            bin.append(false);
        }
        storage /= 2;
    }

    for(int i = bin.size() - 1; i >= 0; i--) {
        if(bin[i]) {
            result += "1";
        } else {
            result += "0";
        }
    }

    return result;
}

QString BlackBoxWindow::focusAndGetText(Gate *gate, int index, bool isInput) {

    QPoint focus = isInput ? gate->getInputLocations()[index] : gate->getOutputLocations()[index];
    focusPoint = focus;
    showRect = true;

    topLeftLocation.setX((int)(focus.x() - width() / 2 / zoom));
    topLeftLocation.setY((int)(focus.y() - height() / 2 / zoom));

    update();
    return getText();
}

QString BlackBoxWindow::getText()
{
    QString text;

    do {
        QEventLoop textLoop;
        connect(ui->textInput, SIGNAL(returnPressed()), &textLoop, SLOT(quit()));
        textLoop.exec();
        text = ui->textInput->text();
    } while(!isValid(text));

    ui->textInput->clear();
    return text;
}

bool BlackBoxWindow::isValid(QString str)
{
    if(str.contains("CONNECTIONS") || str.contains("GATES") || str.contains("INPUTS") || str.contains("OUTPUTS")) {
        return false;
    }

    if(str.contains("AND") || str.contains("CLOCK") || str.contains("OR") || str.contains("NOT") || str.contains("DFLIPFLOP") || str.contains("INPUT") || str.contains("M_INPUT") || str.contains("OUTPUT") || str.contains("DECODER") || str.contains("ENCODER")){
        return false;
    }

    if(str.contains("DRAW WIRE") || str.contains("D-FLIPFLOP") || str.contains("BLACK BOX") || str.contains("INTERACT") || str.contains("MULTI INPUT") || str.contains("MULTI OUTPUT")) {
        return false;
    }

    if(str[0] == '_' || str.contains("~") || str.contains("#") || str.contains("%") || str.contains("&") || str.contains("*") || str.contains("{") || str.contains("}") || str.contains("\\") || str.contains(".") || str.contains(":") || str.contains("<") || str.contains(">") || str.contains("?") || str.contains("/") || str.contains("+") || str.contains("|") || str.contains("\"")) {
        return false;
    }

    return true;
}

void BlackBoxWindow::mouseMoveEvent(QMouseEvent *event)
{
    mouse.setX(event->x());
    mouse.setY(event->y());
    update();
}
