#include "field.h"
#include "ui_field.h"

Field::Field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Field)
{
    setMouseTracking(true);
    ui->setupUi(this);
}

Field::~Field()
{
    delete ui;
}
