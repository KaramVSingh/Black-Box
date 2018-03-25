#ifndef SLIDEBAR_H
#define SLIDEBAR_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class SlideBar;
}

class SlideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SlideBar(QWidget *parent = 0);
    ~SlideBar();

signals:
    void valueChanged(int);

private:
    Ui::SlideBar *ui;
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *e);
    int position = 1000;
    bool dragging = false;
};

#endif // SLIDEBAR_H
