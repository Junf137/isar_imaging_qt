#include "textslider.h"

textSlider::textSlider(QWidget *parent)
    : QSlider(parent)
{
    this->scaleRatio = 1;
//    auto rect = this->geometry();
//    auto fontMetrics = this->fontMetrics();
//    this->setGeometry(rect.x(), rect.y(), rect.width(), rect.height() + fontMetrics.height()*2);
}

textSlider::~textSlider() {}

void textSlider::setScaleRatio(int ratio){
    this->scaleRatio = ratio;
}

float textSlider::realValue(){
    return float(this->value()) / scaleRatio;
}

void textSlider::paintEvent(QPaintEvent* ev){
    QSlider::paintEvent(ev);

    auto painter = new QPainter(this);
    painter->setPen(QPen(Qt::black));

    int numTicks = (maximum() - minimum()) / tickInterval();

    auto fontMetrics = this->fontMetrics();
    auto rect = this->geometry();

    if (this->orientation() == Qt::Horizontal) {

        for (int i=0; i<=numTicks; i++){

            int numText = (minimum() + tickInterval() * i) / scaleRatio;

            auto tickX = ((rect.width()/numTicks) * i) - (fontMetrics.horizontalAdvance(QString::number(numText))/2);
            if(i == 0){
                tickX = ((rect.width()/numTicks) * i);
            }else if(i == numTicks){
                tickX = ((rect.width()/numTicks) * i) - fontMetrics.horizontalAdvance(QString::number(numText));
            }

            auto tickY = rect.height() - this->fontMetrics().height();

            painter->drawText(QPoint(tickX, tickY), QString::number(numText));
        }

    } else if (this->orientation() == Qt::Vertical) {
        for (int i=0; i<=numTicks; i++){

            int numText = (minimum() + tickInterval() * i) / scaleRatio;

            auto tickY = ((rect.height()/numTicks) * i) + (fontMetrics.height()/2);
            if(i == 0){
                tickY = ((rect.height()/numTicks) * numTicks);
            }else if(i == numTicks){
                tickY = fontMetrics.height();
            }

            auto tickX = rect.width() - fontMetrics.horizontalAdvance(QString::number(numText));

            painter->drawText(QPoint(tickX, tickY), QString::number(numText));
        }
    } else {
        return;
    }

//    painter->drawRect(rect);

    delete painter;
}
