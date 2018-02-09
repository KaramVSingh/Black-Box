#include "field.h"
#include "ui_field.h"

Field::Field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Field)
{
    zoom = 1;
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
    }

    update();
}

void Field::mouseMoveEvent(QMouseEvent *e)
{
    QPoint fieldPoint = getFieldLocation(QPoint(e->x(), e->y()));

    if(!dragging) {

    } else {

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
    }
}

void Field::changeTool(Mode newTool, QString data)
{
    tool = newTool;
    toolData = data;
}
