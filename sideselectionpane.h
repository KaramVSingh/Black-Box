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
    void frequencyChanged(int);

public slots:
    void refreshGates();

private slots:
    void on_comboBox_activated(const QString &arg1);

    void on_wireButton_clicked();

    void on_interactButton_clicked();

    void on_decoderBits_returnPressed();

    void on_encoderBits_returnPressed();

    void on_boxButton_clicked();

    void on_frequencySlider_sliderMoved(int position);

private:
    Ui::SideSelectionPane *ui;
    void paintEvent(QPaintEvent *e);
};

#endif // SIDESELECTIONPANE_H
