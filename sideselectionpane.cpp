#include "sideselectionpane.h"
#include "ui_sideselectionpane.h"

SideSelectionPane::SideSelectionPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideSelectionPane)
{
    ui->setupUi(this);
    setMouseTracking(true);

    connect(ui->tools, SIGNAL(pressed(QString)), this, SLOT(optionSelected(QString)));
    connect(ui->standardGates, SIGNAL(pressed(QString)), this, SLOT(optionSelected(QString)));
    connect(ui->customGates, SIGNAL(activated(QString)), this, SLOT(optionSelected(QString)));
    connect(ui->multibitGates, SIGNAL(pressed(QString)), this, SLOT(optionSelected(QString)));

    ui->standardGates->addOption("AND", false);
    ui->standardGates->addOption("OR", false);
    ui->standardGates->addOption("NOT", false);
    ui->standardGates->addOption("D-FLIPFLOP", false);
    ui->standardGates->addOption("CLOCK", false);
    ui->standardGates->addOption("INPUT", false);
    ui->standardGates->addOption("OUTPUT", false);

    ui->multibitGates->addOption("MULTI INPUT", false);
    ui->multibitGates->addOption("MULTI OUTPUT", false);
    ui->multibitGates->addOption("ENCODER", true);
    ui->multibitGates->addOption("DECODER", true);

    ui->tools->addOption("DRAW WIRE", false);
    ui->tools->addOption("INTERACT", false);
    ui->tools->addOption("BLACK BOX", false);

    // add all of the gates
    refreshGates();
    // now should look through all of the gates in the file:
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
    QPen pen(QColor(22, 21, 37));
    pen.setWidth(1);
    paint.setPen(pen);

    // draw the dark rectangle background of the pane
    paint.fillRect(0, 0, this->width(), this->height(), QColor(22, 21, 37));
    paint.drawRect(0, 0, this->width(), this->height());
}

void SideSelectionPane::optionSelected(QString option)
{
    QString fullOption = option;
    option.remove('0');
    option.remove('1');
    option.remove('2');
    option.remove('3');
    option.remove('4');
    option.remove('5');
    option.remove('6');
    option.remove('7');
    option.remove('8');
    option.remove('9');

    if(ui->standardGates->options.contains(option)) {
        ui->multibitGates->selected = -1;
        ui->multibitGates->update();

        ui->tools->selected = -1;
        ui->tools->update();
    }

    if(ui->multibitGates->options.contains(option)) {
        ui->standardGates->selected = -1;
        ui->standardGates->update();

        ui->tools->selected = -1;
        ui->tools->update();
    }

    if(ui->tools->options.contains(option)) {
        ui->standardGates->selected = -1;
        ui->standardGates->update();

        ui->multibitGates->selected = -1;
        ui->multibitGates->update();
    }

    if(ui->standardGates->options.contains(option) || ui->multibitGates->options.contains(option) || ui->tools->options.contains(option)) {
        ui->customGates->setStyleSheet("QComboBox { color : white; background-color : #201f37; border: 0px; } QComboBox::drop-down {border: 0px;}");
    }

    if(ui->standardGates->options.contains(option) || ui->multibitGates->options.contains(option)) {
        emit toolChanged(Mode::place, fullOption);
    } else {
        if(option == "DRAW WIRE") {
            emit toolChanged(Mode::wire, "");
        } else if(option == "INTERACT") {
            emit toolChanged(Mode::interact, "");
        } else if(option == "BLACK BOX") {
            emit toolChanged(Mode::blackBox, "");
        }
    }

}

void SideSelectionPane::refreshGates()
{
    ui->customGates->clear();
    QString dir(QFileInfo(".").absolutePath());
    dir.append("/BlackBox/CustomGates/");
    QDir directory(dir);
    QStringList customGates = directory.entryList(QStringList() << "*.bb" << "*.BB", QDir::Files);

    foreach(QString gate, customGates) {
        QString name = gate.split(".")[0];
        ui->customGates->addItem(name);
    }
}

void SideSelectionPane::on_customGates_activated(const QString &arg1)
{
    ui->customGates->setStyleSheet("QComboBox { color : white; background-color : #1cbbb4; border: 0px; } QComboBox::drop-down {border: 0px;}");

    ui->standardGates->selected = -1;
    ui->standardGates->update();
    ui->multibitGates->selected = -1;
    ui->multibitGates->update();
    ui->tools->selected = -1;
    ui->tools->update();

    emit toolChanged(Mode::place, arg1);
}
