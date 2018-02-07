#include "bottomselectionpane.h"
#include "ui_bottomselectionpane.h"

BottomSelectionPane::BottomSelectionPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BottomSelectionPane)
{
    setMouseTracking(true);
    ui->setupUi(this);
}

BottomSelectionPane::~BottomSelectionPane()
{
    delete ui;
}

// paintEvent will occour whenever the pane is repainted. this will happen upon creation and
// whenever the window is resized:
void BottomSelectionPane::paintEvent(QPaintEvent *e)
{
    // set the painter and the pen for the widget
    QPainter paint(this);
    QPen pen(QColor(50, 50, 50));
    pen.setWidth(4);
    paint.setPen(pen);

    // draw the dark rectangle background of the pane
    paint.fillRect(0, 0, this->width(), this->height(), QColor(70, 70, 70));
    paint.drawRect(-4, 0, this->width() + 4, this->height());
}
