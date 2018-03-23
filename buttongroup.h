#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonGroup(QWidget *parent = 0);
    void addOption(QString option, bool takesInt);
    ~ButtonGroup();
    int selected = -1;
    QVector<QString> options;

signals:
    void pressed(QString option);

private:
    Ui::ButtonGroup *ui;
    bool hasMouse = false;
    int highlight = 0;
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    QVector<bool> takesInts;

    QString enteredNumber = "0";
    bool isAcceptingTyping = false;

};

#endif // BUTTONGROUP_H
