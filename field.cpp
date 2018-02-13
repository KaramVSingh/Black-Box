#include "field.h"
#include "ui_field.h"

Field::Field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Field)
{
    zoom = 1;
    drawingWire = false;
    tool = Mode::place;
    dragging = false;
    setMouseTracking(true);
    ui->setupUi(this);
    topLeftLocation.setX(0);
    topLeftLocation.setY(0);
}

Field::~Field()
{
    delete ui;
}

// mouse press event will work for whenever you are simply pressing
void Field::mousePressEvent(QMouseEvent *e)
{
    dragging = true;
    // location will be in terms of the field, not the contents of the field
    QPoint fieldPoint = getFieldLocation(QPoint(e->x(), e->y()));
    startLocation = fieldPoint;

    switch(tool) {
    case Mode::place:
        placeGate(fieldPoint);
        break;
    case Mode::zoomIn:
        if(zoom < 2) {
            zoom *= 2;
            // we also need to change the topLeftLocation. we want to have the width and
            // the height in terms of the new grid density. if we zoom in it is 32px
            int windowWidth = this->width() / GRID_DENSITY * GRID_DENSITY;
            int windowHeight = this ->height() / GRID_DENSITY * GRID_DENSITY;

            topLeftLocation.setX(fieldPoint.x() - (int)(windowWidth / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
            topLeftLocation.setY(fieldPoint.y() - (int)(windowHeight / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
        }
        break;
    case Mode::zoomOut:
        if(zoom > 0.5) {
            zoom /= 2;
            // we also need to change the topLeftLocation
            int windowWidth = this->width() / GRID_DENSITY * GRID_DENSITY;
            int windowHeight = this ->height() / GRID_DENSITY * GRID_DENSITY;

            topLeftLocation.setX(fieldPoint.x() - (int)(windowWidth / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
            topLeftLocation.setY(fieldPoint.y() - (int)(windowHeight / 2 / zoom) / GRID_DENSITY * GRID_DENSITY);
        }
        break;
    case Mode::wire:
        drawWire(fieldPoint);
        break;
    case Mode::interact:
        toggleInputs(fieldPoint);
    }

    update();
}

void Field::mouseMoveEvent(QMouseEvent *e)
{
    QPoint fieldPoint = getFieldLocation(QPoint(e->x(), e->y()));
    int windowWidth = this->width() / GRID_DENSITY * GRID_DENSITY;
    int windowHeight = this ->height() / GRID_DENSITY * GRID_DENSITY;

    if(!dragging) {

    } else {

    }
    update();
}

void Field::mouseReleaseEvent(QMouseEvent *e)
{
    dragging = false;
    QPoint fieldPoint = getFieldLocation(QPoint(e->x(), e->y()));

    switch(tool) {
    case Mode::move:
        topLeftLocation += startLocation - fieldPoint;
        break;
    }
    update();
}

// paintEvent should draw all gates and wires in the right location with the right zoom
void Field::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    QPen newPen(Qt::black);
    newPen.setWidth((int)(2 * zoom));
    paint.setPen(newPen);

    paint.fillRect(0, 0, this->width(), this->height(), Qt::white);
    for(int i = 0; i < gates.size(); i++) {
        paint.drawImage((gates[i]->location - topLeftLocation) * zoom, gates[i]->toImage(zoom));
    }

    for(int i = 0; i < wires.size(); i++) {

        if(!wires[i]->moreThanOneVertex) {
            continue;
        }

        for(int j = 0; j < wires[i]->vertices.size() - 1; j++) {
            paint.drawLine((wires[i]->vertices[j] - topLeftLocation) * zoom, (wires[i]->vertices[j + 1] - topLeftLocation) * zoom);
        }
    }
}

// used to translate between gui point and field point
QPoint Field::getFieldLocation(QPoint guiLocation)
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

void Field::placeGate(QPoint location)
{
    if(toolData == "AND") {
        gates.append(new And(location));
    } else if(toolData == "NOT") {
        gates.append(new Not(location));
    } else if(toolData == "OR") {
        gates.append(new Or(location));
    } else if(toolData == "INPUT") {
        Input* newInput = new Input(location);
        gates.append(newInput);
        inputGates.append(newInput);
    } else if(toolData == "OUTPUT") {
        Output* newOutput = new Output(location);
        gates.append(newOutput);
        outputGates.append(newOutput);
    }
}

void Field::changeTool(Mode newTool, QString data)
{
    tool = newTool;
    toolData = data;
}

void Field::drawWire(QPoint point)
{
    // first thing you want to do is establish if this wire is the start of a new wire or the
    // continuation of an old one

    if(drawingWire) {
        // the wire is a continuation of an old wire:

        // there are four possibilities, you connect to an output, you connect to an input, you
        // connect to a wire, or you connect to nothing
        for(int i = 0; i < gates.size(); i++) {
            QVector<QPoint> inputPorts = gates[i]->getInputLocations();
            if(inputPorts.contains(point)) {
                // we are not going to allow connections directly from inputs to inputs
                // or from outputs to outputs
                Wire* currentWire = wires[wires.size() - 1];
                if(currentWire->hasOutput()) {
                    // we do not want to complete the wire by adding the vertex
                    return;
                } else {
                    if(gates[i]->takenInputs.contains(inputPorts.indexOf(point))) {
                        // if this is the case then something has already been hooked up to that port
                        return;
                    } else {
                        // this means we can safely connect the wire
                        drawingWire = false;
                        currentWire->addVertex(point);
                        currentWire->moreThanOneVertex = true;
                        currentWire->addOutputConnection(gates[i], inputPorts.indexOf(point));

                        // now we need to connect the gates:
                        Gate* inputOfWire = currentWire->input.gate;
                        int inputOfWireIndex = currentWire->input.otherIndex;

                        Gate* outputOfWire = currentWire->output.gate;
                        int outputOfWireIndex = currentWire->output.otherIndex;

                        inputOfWire->addOutput(outputOfWire, inputOfWireIndex, outputOfWireIndex);
                        outputOfWire->addInput(inputOfWire, outputOfWireIndex, inputOfWireIndex);
                        return;
                    }
                }
            }

            // if we are here we know that it is not connected to an input port
            QVector<QPoint> outputPorts = gates[i]->getOutputLocations();
            if(outputPorts.contains(point)) {
                // we are not going to allow connections directly from inputs to inputs
                // or from outputs to outputs
                Wire* currentWire = wires[wires.size() - 1];
                if(currentWire->hasInput()) {
                    // we do not want to complete the wire
                    return;
                } else {
                    if(gates[i]->takenOutputs.contains(outputPorts.indexOf(point))) {
                        // then there is something already hooked up to that port:
                        return;
                    } else {
                        // this means we can safely connect the wire
                        drawingWire = false;
                        currentWire->addVertex(point);
                        currentWire->moreThanOneVertex = true;
                        currentWire->addInputConnection(gates[i], outputPorts.indexOf(point));

                        // now we need to connect the gates:
                        Gate* inputOfWire = currentWire->input.gate;
                        int inputOfWireIndex = currentWire->input.otherIndex;

                        Gate* outputOfWire = currentWire->output.gate;
                        int outputOfWireIndex = currentWire->output.otherIndex;

                        inputOfWire->addOutput(outputOfWire, inputOfWireIndex, outputOfWireIndex);
                        outputOfWire->addInput(inputOfWire, outputOfWireIndex, inputOfWireIndex);
                        return;
                    }
                }
            }
        }

        // if we are here we knoe that it is not connected to a gate. the two remaining options is
        // to a wire and to nothing. we will handle the wire case and what ever is left will
        // by definition be connected to nothing

        for(int i = 0; i < wires.size(); i++) {
            if(wires[i]->vertices.contains(point)) {
                // in this case we are attempting to connect to a wire.
                // we allow connection to a wire when the connection is to
                Wire* currentWire = wires[wires.size() - 1];
                if(currentWire->hasOutput()) {
                    // if the current wire has an output then it is only connected
                    // to the input port of a gate, which means it can connect anywhere
                    drawingWire = false;
                    currentWire->addVertex(point);
                    currentWire->moreThanOneVertex = true;
                    currentWire->addInputConnection(wires[i]->input.gate, wires[i]->input.otherIndex);

                    // now we need to connect the gates:
                    Gate* inputOfWire = currentWire->input.gate;
                    int inputOfWireIndex = currentWire->input.otherIndex;

                    Gate* outputOfWire = currentWire->output.gate;
                    int outputOfWireIndex = currentWire->output.otherIndex;

                    inputOfWire->addOutput(outputOfWire, inputOfWireIndex, outputOfWireIndex);
                    outputOfWire->addInput(inputOfWire, outputOfWireIndex, inputOfWireIndex);
                    return;
                }

                if(currentWire->hasInput()) {
                    // if the wire has an input, that means it is connected to the output port of a gate,
                    // in which case it can only connect to a wire with only inputs which is temporarily banned
                    // therefore nothing should be allowed
                    return;
                }
            }
        }

        // if we are here than it is just adding a new vertex in which we do not hve to perform any checks
        Wire* currentWire = wires[wires.size() - 1];
        currentWire->moreThanOneVertex = true;
        currentWire->addVertex(point);
        return;

    } else {
        // this is a new wire:

        // first thing you need to check for a new wire is if it is being connected to a gate:
        // first we will check if the point is the same as an input/output port of a gate
        for(int i = 0; i < gates.size(); i++) {
            QVector<QPoint> inputPorts = gates[i]->getInputLocations();
            if(inputPorts.contains(point)) {
                // then we are connecting to a gate by the input port, which means the gate is now a
                // output of the wire
                if(gates[i]->takenInputs.contains(inputPorts.indexOf(point))) {
                    // if this is the case then the gate already has something hooked up to this port
                    return;
                } else {
                    drawingWire = true;
                    Wire* newWire = new Wire();
                    wires.append(newWire);
                    newWire->addVertex(point);
                    newWire->addOutputConnection(gates[i], inputPorts.indexOf(point));
                    return;
                }
            }

            // if we are here we know it isnt connected to an input port so we should check output ports
            QVector<QPoint> outputPorts = gates[i]->getOutputLocations();
            if(outputPorts.contains(point)) {
                // then we are connecting to a gate by the output port, which means the gate is now a
                // input of the wire
                if(gates[i]->takenOutputs.contains(outputPorts.indexOf(point))) {
                    // if this is the case, then the gate already has something connected to taht port
                    return;
                } else {
                    drawingWire = true;
                    Wire* newWire = new Wire();
                    wires.append(newWire);
                    newWire->addVertex(point);
                    newWire->addInputConnection(gates[i], outputPorts.indexOf(point));
                    return;
                }
            }
        }

        // if we are here, we know it is a new wire, however we are not connected to a gate. the only
        // other possibility is that we are connected to a vertex of a wire
        for(int i = 0; i < wires.size(); i++) {
            QVector<QPoint> vertices = wires[i]->getVertices();
            if(vertices.contains(point)) {
                // then we are connecting to a wire by a vertex. there are 2 circumstances in which we
                // could connect to a wire. only one of which constitutes creating a new wire
                drawingWire = true;
                Wire* newWire = new Wire();
                wires.append(newWire);
                newWire->addVertex(point);
                newWire->addInputConnection(wires[i]->input.gate, wires[i]->input.otherIndex);
                return;
            }
        }
    }
}

void Field::toggleInputs(QPoint point)
{
    for(int i = 0; i < inputGates.size(); i++) {
        if(point == inputGates[i]->location + QPoint(GRID_DENSITY, GRID_DENSITY)) {
            // then we say you have pressed on the input:
            inputGates[i]->changeValue();

            for(int j = 0; j < outputGates.size(); j++) {
                outputGates[j]->execute(0);
            }

            return;
        }
    }
}
