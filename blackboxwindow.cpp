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
}

void BlackBoxWindow::on_moveButton_clicked()
{
    tool = Mode::move;
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
