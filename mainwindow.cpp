#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sideselectionpane, SIGNAL(toolChanged(Mode,QString)), ui->field, SLOT(changeTool(Mode,QString)));
    connect(ui->topselectionpane, SIGNAL(toolChanged(Mode,QString)), ui->field, SLOT(changeTool(Mode,QString)));
    connect(ui->field, SIGNAL(addedGate()), ui->sideselectionpane, SLOT(refreshGates()));
    connect(ui->sideselectionpane, SIGNAL(frequencyChanged(int)), ui->field, SLOT(changeFrequency(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
