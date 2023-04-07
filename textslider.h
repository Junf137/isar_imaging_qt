#ifndef TEXTSLIDER_H
#define TEXTSLIDER_H

#include <QSlider>
#include <QPainter>
#include <QPen>

#include <QDebug>

class textSlider : public QSlider
{
    Q_OBJECT
private:
    int scaleRatio;

public:
    textSlider(QWidget *parent = nullptr);
    ~textSlider();

    float realValue();

    void setScaleRatio(int ratio);
    void paintEvent(QPaintEvent* ev);
};

#endif // TEXTSLIDER_H
