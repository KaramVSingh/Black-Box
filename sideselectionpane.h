#ifndef SIDESELECTIONPANE_H
#define SIDESELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "detail.h"

namespace Ui {
class SideSelectionPane;
}

class SideSelectionPane : public QWidget
{
    Q_OBJECT

public:
    explicit SideSelectionPane(QWidget *parent = 0);
    ~SideSelectionPane();

signals:
    void toolChanged(Mode, QString);

public slots:
    void refreshGates();
    void optionSelected(QString option);

private slots:
    void on_customGates_activated(const QString &arg1);

private:
    Ui::SideSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // SIDESELECTIONPANE_H
