#ifndef TOPSELECTIONPANE_H
#define TOPSELECTIONPANE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QPen>

#include "detail.h"

namespace Ui {
class TopSelectionPane;
}

class TopSelectionPane : public QWidget
{
    Q_OBJECT

public:
    explicit TopSelectionPane(QWidget *parent = 0);
    ~TopSelectionPane();

private slots:
    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_moveButton_clicked();

signals:
    void toolChanged(Mode newTool, QString data);

private:
    Ui::TopSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // TOPSELECTIONPANE_H
