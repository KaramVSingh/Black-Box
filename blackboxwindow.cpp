#include "blackboxwindow.h"
#include "ui_blackboxwindow.h"

BlackBoxWindow::BlackBoxWindow(QVector<Gate*> gates, QVector<Wire*> wires, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlackBoxWindow)
{
    ui->setupUi(this);
    topLeftLocation = QPoint(0, 0);
    this->gates = gates;
    this->wires = wires;
    update();
}

BlackBoxWindow::~BlackBoxWindow()
{
    delete ui;
}

void BlackBoxWindow::mousePressEvent(QMouseEvent *event)
{
    startingPoint = getFieldLocation(QPoint(event->x(), event->y()));
    update();
    switch(tool) {
    case Mode::zoomIn:
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
    case Mode::zoomOut:
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
    switch(tool) {
    case Mode::move:
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
    QString fileName = gateName + ".bb";

    QVector<Gate*> fullGates;

    QString gateDefinition = "";
    gateDefinition += "GATES = ";

    // we want to create an array of basic gates
    for(int i = 0; i < gates.size(); i++) {
        if(gates[i]->toType() != GateType::CUSTOM) {
            fullGates.append(gates[i]);
        } else {
            CustomGate* gate = static_cast<CustomGate*>(gates[i]);
            fullGates.append(gate->internalGates);
            blackBoxGates.append(gate->internalGates);
        }
    }

    gateDefinition += QString::number(fullGates.size()) + ":\r\n";

    for(int i = 0; i < fullGates.size(); i++) {
        gateDefinition += QString::number(i) + ". " + fullGates[i]->toString() + "\r\n";
    }

    gateDefinition += "~\r\n";
    gateDefinition += "CONNECTIONS:\r\n";

    for(int i = 0; i < fullGates.size(); i++) {
        for(int j = 0; j < fullGates[i]->takenInputs.size(); j++) {
            int inputIndex = fullGates[i]->takenInputs[j];
            Gate::Connection inputConnection = fullGates[i]->inputs[inputIndex];
            if(fullGates.contains(inputConnection.gate)) {
                gateDefinition += QString::number(fullGates.indexOf(inputConnection.gate)) + ",";
                gateDefinition += QString::number(inputConnection.otherIndex) + " -> ";
                gateDefinition += QString::number(i) + "," + QString::number(inputIndex) + "\r\n";
            }
        }
    }

    gateDefinition += "~\r\n";
    gateDefinition += "INPUTS = ";
    QString inputsString = "";

    int numberOfInputs = 0;
    QVector<Gate::Connection> handledOpenBlackBoxPort;
    for(int i = 0; i < fullGates.size(); i++) {
        for(int j = 0; j < fullGates[i]->numberOfInputLines; j++) {
            if(fullGates[i]->takenInputs.contains(j)) {
                if(!fullGates.contains(fullGates[i]->inputs[j].gate)) {
                    // we need to look at all of the outputs for the inputs so as to see if we have
                    // already looked at this input
                    int indexOfThisOutput = fullGates[i]->inputs[j].otherIndex;
                    QList<Gate::Connection> outsOfIn = fullGates[i]->inputs[j].gate->outputs[indexOfThisOutput];
                    bool hasBeenHandled = false;
                    // the list contains every output of the gate that is connected to that point
                    for(int k = 0; k < i; k++) {
                        if(fullGates.contains(outsOfIn[k].gate)) {
                            hasBeenHandled = true;
                            break;
                        } else {
                            continue;
                        }
                    }

                    if(!hasBeenHandled) {
                        QString name = focusAndGetText(fullGates[i], j, true);

                        if(name == "") {
                            continue;
                        }

                        inputsString += QString::number(numberOfInputs++) + ". " + name + "~";
                        for(int k = 0; k < outsOfIn.size(); k++) {
                            if(fullGates.contains(outsOfIn[k].gate)) {
                                inputsString += " -> " + QString::number(fullGates.indexOf(outsOfIn[k].gate)) + "," + QString::number(outsOfIn[k].otherIndex);
                            }
                        }

                        inputsString += "~\r\n";
                    }
                }
            } else {
                // there is nothing connected to this port therefore it is an input.
                // we have two options, ignore it and not add it or add it.
                // it can only possibly be connected to this gate

                int blackBoxNumber;
                bool isInBlackBox;
                // we have to also set all of the inputs connected to the same port if that is an issue
                for(int k = 0; k < blackBoxGates.size(); k++) {
                    if(blackBoxGates[k].contains(fullGates[i])) {
                        isInBlackBox = true;
                        blackBoxNumber = k;
                    }
                }

                if(isInBlackBox) {
                    // the we need to apply the change we make to this input to all of the input ports

                    // we need all of the gates connected to this port
                    int num = 0;
                    for(int k = 0; k < gates.size(); k++) {
                        if(gates[k]->toType() == GateType::CUSTOM) {
                            if(num == blackBoxNumber) {
                                CustomGate* tempGate = static_cast<CustomGate*>(gates[k]);

                                int portIndex = 0;
                                for(int l = 0; l < tempGate->inputPointers.size(); l++) {
                                    for(int r = 0; r < tempGate->inputPointers[l].size(); r++) {
                                        if(tempGate->inputPointers[l][r].gate == fullGates[i]) {
                                            if(tempGate->inputPointers[l][r].otherIndex == j) {
                                                portIndex = l;
                                            }
                                        }
                                    }
                                }

                                QList<Gate::Connection> port = tempGate->inputPointers[portIndex];
                                // we should add input for all of these

                                bool alreadyHandled = false;
                                for(int l = 0; l < handledOpenBlackBoxPort.size(); l++) {
                                    if(handledOpenBlackBoxPort[l].gate == port[0].gate) {
                                        if(handledOpenBlackBoxPort[l].otherIndex == port[0].otherIndex) {
                                            alreadyHandled = true;
                                            break;
                                        }
                                    }
                                }

                                if(alreadyHandled) {
                                    break;
                                }

                                QString name = focusAndGetText(fullGates[i], j, true);
                                inputsString += QString::number(numberOfInputs++) + ". ";
                                inputsString += name + "~";

                                if(name == "") {
                                    continue;
                                }

                                for(int l = 0; l < port.size(); l++) {
                                    inputsString += " -> " + QString::number(fullGates.indexOf(port[l].gate)) + "," + QString::number(port[l].otherIndex);
                                    Gate::Connection handledOpenPort;
                                    handledOpenPort.gate = port[l].gate;
                                    handledOpenPort.otherIndex = port[l].otherIndex;
                                    handledOpenBlackBoxPort.append(handledOpenPort);
                                }

                                inputsString += "~\r\n";
                                break;
                            } else {
                                num++;
                            }
                        }
                    }

                } else {
                    QString name = focusAndGetText(fullGates[i], j, true);

                    if(name == "") {
                        continue;
                    }

                    inputsString += QString::number(numberOfInputs++) + ". ";
                    inputsString += name + "~ -> " + QString::number(i) + "," + QString::number(j) + "~\r\n";
                }
            }
        }
    }

    inputsString += "~\r\n";
    gateDefinition += QString::number(numberOfInputs) + ":\r\n" + inputsString;

    gateDefinition += "OUTPUTS = ";
    QString outputsString = "";
    int numberOfOutputs = 0;

    for(int i = 0; i < fullGates.size(); i++) {
        for(int j = 0; j < fullGates[i]->outputs.size(); j++) {
            if(fullGates[i]->takenOutputs.contains(j)) {
                for(int k = 0; k < fullGates[i]->outputs[j].size(); k++) {
                    Gate::Connection outs = fullGates[i]->outputs[j][k];
                    if(fullGates.contains(outs.gate)) {
                        if(outs.gate->toType() == GateType::OUTPUT) {
                            // we are elligible to add an output here:
                            QString name = focusAndGetText(fullGates[i], j, false);

                            if(name == "") {
                                continue;
                            }

                            outputsString += QString::number(numberOfOutputs++) + ". " + name + "~ <- " + QString::number(i) + "," + QString::number(j) + "\r\n";
                        } else {
                            continue;
                        }
                    } else {
                        // we are elligible to add an output here:
                        QString name = focusAndGetText(fullGates[i], j, false);

                        if(name == "") {
                            continue;
                        }

                        outputsString += QString::number(numberOfOutputs++) + ". " + name + "~ <- " + QString::number(i) + "," + QString::number(j) + "\r\n";
                    }
                }
            } else {
                // we are elligible to add an output here:
                QString name = focusAndGetText(fullGates[i], j, false);

                if(name == "") {
                    continue;
                }

                outputsString += QString::number(numberOfOutputs++) + ". " + name + "~ <- " + QString::number(i) + "," + QString::number(j) + "\r\n";
            }
        }
    }

    outputsString += "~";
    gateDefinition += QString::number(numberOfOutputs) + ":\r\n" + outputsString;


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

QString BlackBoxWindow::focusAndGetText(Gate *gate, int index, bool isInput) {
    bool isInBlackBox;
    int blackBoxNumber;

    for(int i = 0; i < blackBoxGates.size(); i++) {
        if(blackBoxGates[i].contains(gate)) {
            isInBlackBox = true;
            blackBoxNumber = i;
        }
    }

    if(isInBlackBox) {
        int num = 0;
        for(int i = 0; i < gates.size(); i++) {
            if(gates[i]->toType() == GateType::CUSTOM) {
                if(num == blackBoxNumber) {
                    CustomGate* customGate = static_cast<CustomGate*>(gates[i]);
                    if(isInput) {
                        for(int j = 0; j < customGate->inputPointers.size(); j++) {
                            for(int k = 0; k < customGate->inputPointers[j].size(); k++) {
                                if(customGate->inputPointers[j][k].gate == gate) {
                                    if(customGate->inputPointers[j][k].otherIndex == index) {
                                        index = j;
                                    }
                                }
                            }
                        }
                    } else {
                        for(int j = 0; j < customGate->outputPointers.size(); j++) {
                            if(customGate->outputPointers[j].gate == gate) {
                                if(customGate->outputPointers[j].otherIndex == index) {
                                    index = j;
                                }
                            }
                        }
                    }
                    gate = gates[i];
                    break;
                }
                num++;
            }
        }
    }

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

    if(str.contains("AND") || str.contains("OR") || str.contains("NOT") || str.contains("DFLIPFLOP") || str.contains("INPUT") || str.contains("M_INPUT") || str.contains("OUTPUT") || str.contains("DECODER") || str.contains("ENCODER")){
        return false;
    }

    if(str[0] == '_' || str.contains("~") || str.contains("#") || str.contains("%") || str.contains("&") || str.contains("*") || str.contains("{") || str.contains("}") || str.contains("\\") || str.contains(".") || str.contains(":") || str.contains("<") || str.contains(">") || str.contains("?") || str.contains("/") || str.contains("+") || str.contains("|") || str.contains("\"")) {
        return false;
    }
    return true;
}

void BlackBoxWindow::on_zoomInButton_clicked()
{
    tool = Mode::zoomIn;
}

void BlackBoxWindow::on_zoomOutButton_clicked()
{
    tool = Mode::zoomOut;
}

void BlackBoxWindow::on_moveButton_clicked()
{
    tool = Mode::move;
}
