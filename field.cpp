#include "field.h"
#include "ui_field.h"

Field::Field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Field)
{
    zoom = 1;
    tool = mode::place;
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
    case mode::place:
        gates.append(new And(fieldPoint));
        tool = mode::zoomIn;
        break;
    case mode::zoomIn:
        if(zoom < 2) {
            zoom *= 2;
            // we also need to change the topLeftLocation
            int windowWidth = this->width() / 16 * 16;
            int windowHeight = this ->height() / 16 * 16;

            topLeftLocation.setX(fieldPoint.x() - windowWidth / 2 / zoom);
            topLeftLocation.setY(fieldPoint.y() - windowHeight / 2 / zoom);
        }
        break;
    }

    update();
}

void Field::mouseMoveEvent(QMouseEvent *e)
{
    QPoint fieldPoint = getFieldLocation(QPoint(e->x(), e->y()));

    if(!dragging) {

    }
    update();
}

void Field::mouseReleaseEvent(QMouseEvent *e)
{
    dragging = false;
}

// paintEvent should draw all gates and wires in the right location with the right zoom
void Field::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    QPen newPen(Qt::black);
    newPen.setWidth(2);
    paint.setPen(newPen);

    paint.fillRect(0, 0, this->width(), this->height(), Qt::white);
    for(int i = 0; i < gates.size(); i++) {
        paint.drawImage((gates[i]->location - topLeftLocation) * zoom, gates[i]->toImage(zoom));
    }
}

// used to translate between gui point and field point
QPoint Field::getFieldLocation(QPoint guiLocation)
{
    // now we need to find the point that represents the location on the field
    QPoint fieldLocation(topLeftLocation + guiLocation);
    fieldLocation.setX(fieldLocation.x() / zoom);
    fieldLocation.setY(fieldLocation.y() / zoom);

    // we want to place it on the grid:
    if(fieldLocation.x() % GRID_DENSITY < GRID_DENSITY / 2 * zoom) {
        fieldLocation.setX(fieldLocation.x() - fieldLocation.x() % GRID_DENSITY * zoom);
    } else {
        fieldLocation.setX(fieldLocation.x() + GRID_DENSITY * zoom - fieldLocation.x() % GRID_DENSITY * zoom);
    }

    if(fieldLocation.y() % GRID_DENSITY < GRID_DENSITY / 2 * zoom) {
        fieldLocation.setY(fieldLocation.y() - fieldLocation.y() % GRID_DENSITY * zoom);
    } else {
        fieldLocation.setY(fieldLocation.y() + GRID_DENSITY * zoom - fieldLocation.y() % GRID_DENSITY * zoom);
    }

    return fieldLocation;
}
