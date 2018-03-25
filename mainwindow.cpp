#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sideselectionpane, SIGNAL(toolChanged(Mode,QString)), ui->field, SLOT(changeTool(Mode,QString)));
    connect(ui->field, SIGNAL(addedGate()), ui->sideselectionpane, SLOT(refreshGates()));
    // connect(ui->sideselectionpane, SIGNAL(frequencyChanged(int)), ui->field, SLOT(changeFrequency(int)));

    pane = new TopSelectionPane(this);
    pane->move(width() - pane->width(), ui->menuBar->height());
    pane->show();

    pane->init(ui->field);
    connect(pane, SIGNAL(toolChanged(Mode,QString)), ui->field, SLOT(changeTool(Mode,QString)));
    connect(ui->sideselectionpane, SIGNAL(toolChanged(Mode,QString)), pane, SLOT(clearSelected(Mode,QString)));
    connect(pane, SIGNAL(toolChanged(Mode,QString)), ui->sideselectionpane, SLOT(clearSelected(Mode,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    pane->move(width() - pane->width(), ui->menuBar->height());
}
