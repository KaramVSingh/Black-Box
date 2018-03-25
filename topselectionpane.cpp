#include "topselectionpane.h"
#include "ui_topselectionpane.h"

TopSelectionPane::TopSelectionPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopSelectionPane)
{
    setMouseTracking(true);
    ui->setupUi(this);
    QString dir(QFileInfo(".").absolutePath());
    QPixmap pixMove(dir + "/BlackBox/imageSources/items/move_icon.png");
    moveIcon = pixMove.toImage();
    QPixmap pixZoomIn(dir + "/BlackBox/imageSources/items/zoom_icon.png");
    zoomInIcon = pixZoomIn.toImage();
    QPixmap pixZoomOut(dir + "/BlackBox/imageSources/items/zoomOut_icon.png");
    zoomOutIcon = pixZoomOut.toImage();
}

void TopSelectionPane::init(Field *widget)
{
    connect(ui->frequencyBar, SIGNAL(valueChanged(int)), widget, SLOT(changeFrequency(int)));
}

TopSelectionPane::~TopSelectionPane()
{
    delete ui;
}

void TopSelectionPane::clearSelected(Mode m, QString s)
{
    selected = -1;
    update();
}

// paintEvent will occour whenever the pane is repainted. this will happen upon creation and
// whenever the window is resized:
void TopSelectionPane::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.fillRect(0, 25, 50, 50, QColor(39, 38, 67));
    paint.fillRect(75, 25, 50, 50, QColor(39, 38, 67));
    paint.fillRect(150, 25, 50, 50, QColor(39, 38, 67));

    if(hasMouse && QRect(0, 25, 50, 50).contains(mouse)) {
        paint.fillRect(0, 25, 50, 50, QColor(58, 78, 100));
    }

    if(hasMouse && QRect(75, 25, 50, 50).contains(mouse)) {
        paint.fillRect(75, 25, 50, 50, QColor(58, 78, 100));
    }

    if(hasMouse && QRect(150, 25, 50, 50).contains(mouse)) {
        paint.fillRect(150, 25, 50, 50, QColor(58, 78, 100));
    }

    if(selected != -1) {
        paint.fillRect(selected * 75, 25, 50, 50, QColor(28, 187, 180));
    }

    paint.drawImage(0, 25, moveIcon);
    paint.drawImage(75, 25, zoomInIcon);
    paint.drawImage(150, 25, zoomOutIcon);
}

void TopSelectionPane::mouseMoveEvent(QMouseEvent *e)
{
    hasMouse = true;
    mouse.setX(e->x());
    mouse.setY(e->y());
    update();
}

void TopSelectionPane::mousePressEvent(QMouseEvent *e)
{
    if(QRect(0, 25, 50, 50).contains(mouse)) {
        selected = 0;
        emit toolChanged(Mode::move, "");
    }

    if(QRect(75, 25, 50, 50).contains(mouse)) {
        selected = 1;
        emit toolChanged(Mode::zoomIn, "");
    }

    if(QRect(150, 25, 50, 50).contains(mouse)) {
        selected = 2;
        emit toolChanged(Mode::zoomOut, "");
    }

    update();
}

void TopSelectionPane::enterEvent(QEvent *e)
{
    hasMouse = true;
}

void TopSelectionPane::leaveEvent(QEvent *e)
{
    hasMouse = false;
}
