#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::addOption(QString option, bool takesInt)
{
    options.append(option);
    setMinimumHeight(options.size() * 28);
    takesInts.append(takesInt);
}

void ButtonGroup::mouseMoveEvent(QMouseEvent *e)
{
    hasMouse = true;
    highlight = e->y() / 28;
    update();
}

void ButtonGroup::enterEvent(QEvent *e)
{
    hasMouse = true;
}

void ButtonGroup::leaveEvent(QEvent *e)
{
    hasMouse = false;
    update();
}

void ButtonGroup::mousePressEvent(QMouseEvent *e)
{
    selected = e->y() / 28;
    emit pressed(options[selected]);
    selected = e->y() / 28;
    update();

    if(takesInts[selected]) {

    }
}

void ButtonGroup::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.fillRect(0, 0, width(), height(), QColor(39, 38, 67));
    paint.setPen(QColor(255, 255, 255));
    QFont font("ISOCP");
    font.setPixelSize(20);
    paint.setFont(font);

    if(selected != -1) {
        paint.fillRect(0, selected * 28, width(), 28, QColor(28, 187, 180));
    }

    for(int i = 0; i < options.size(); i++) {
        if(i == highlight && hasMouse && i != selected) {
            paint.fillRect(0, highlight * 28, width(), 28, QColor(58, 78, 100));
        }

        paint.drawText(10, 20 + 28 * i, options[i]);
    }
}
