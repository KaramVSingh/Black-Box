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
}
