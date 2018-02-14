#include "sideselectionpane.h"
#include "ui_sideselectionpane.h"

SideSelectionPane::SideSelectionPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideSelectionPane)
{
    setMouseTracking(true);
    ui->setupUi(this);

    // add all of the gates
    ui->comboBox->addItem("AND");
    ui->comboBox->addItem("OR");
    ui->comboBox->addItem("NOT");
    ui->comboBox->addItem("INPUT");
    ui->comboBox->addItem("M_INPUT");
    ui->comboBox->addItem("OUTPUT");
}

SideSelectionPane::~SideSelectionPane()
{
    delete ui;
}

// paintEvent will occour whenever the pane is repainted. this will happen upon creation and
// whenever the window is resized:
void SideSelectionPane::paintEvent(QPaintEvent *e)
{
    // set the painter and the pen for the widget
    QPainter paint(this);
    QPen pen(QColor(50, 50, 50));
    pen.setWidth(4);
    paint.setPen(pen);

    // draw the dark rectangle background of the pane
    paint.fillRect(0, 0, this->width(), this->height(), QColor(70, 70, 70));
    paint.drawRect(0, 0, this->width(), this->height());
}

void SideSelectionPane::on_comboBox_activated(const QString &arg1)
{
    emit toolChanged(Mode::place, arg1);
}

void SideSelectionPane::on_wireButton_clicked()
{
    emit toolChanged(Mode::wire, "");
}

void SideSelectionPane::on_interactButton_clicked()
{
    emit toolChanged(Mode::interact, "");
}

void SideSelectionPane::on_decoderBits_returnPressed()
{
    emit toolChanged(Mode::place, "DECODER" + ui->decoderBits->text());
}

void SideSelectionPane::on_encoderBits_returnPressed()
{
    emit toolChanged(Mode::place, "ENCODER" + ui->encoderBits->text());
}
