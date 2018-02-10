#ifndef SIDESELECTIONPANE_H
#define SIDESELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>

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

private slots:
    void on_comboBox_activated(const QString &arg1);

    void on_wireButton_clicked();

private:
    Ui::SideSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // SIDESELECTIONPANE_H
