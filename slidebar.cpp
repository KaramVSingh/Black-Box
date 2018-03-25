#include "slidebar.h"
#include "ui_slidebar.h"

SlideBar::SlideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlideBar)
{
    ui->setupUi(this);
}

SlideBar::~SlideBar()
{
    delete ui;
}

void SlideBar::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);

    for(int i = 0; i < 100; i++) {
        paint.fillRect(i * width() / 100, 5, (i + 1) * width() / 100, 5, QColor(255 - (255 - 39) * i / 100, 255 - (255 - 38) * i / 100, 255 - (255 - 67) * i / 100));
    }

    paint.fillRect((1000 - position) * (width() - 5) / 1000, 0, 5, 15, QColor(28, 187, 180));
}

void SlideBar::mousePressEvent(QMouseEvent *e)
{
    dragging = true;
    position = 1000 - (int)((double)(e->x()) / width() * 1000);
    if(position < 0) {
        position = 0;
    }

    if(position > 1000) {
        position = 1000;
    }

    emit valueChanged(position);
    update();
}

void SlideBar::mouseMoveEvent(QMouseEvent *e)
{
    if(dragging) {
        position = 1000 - (int)((double)(e->x()) / width() * 1000);
        if(position < 0) {
            position = 0;
        }

        if(position > 1000) {
            position = 1000;
        }

        emit valueChanged(position);
    }

    update();
}

void SlideBar::mouseReleaseEvent(QMouseEvent *e)
{
    dragging = false;
}

void SlideBar::leaveEvent(QEvent *e)
{
    dragging = false;
}
