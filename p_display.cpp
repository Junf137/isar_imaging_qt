#include <QStyleOption>
#include "p_display.h"

#include <QtMath>

PDisplay::PDisplay(QWidget *parent)
	: QWidget(parent)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    pieAngle = 0;
    speedInterval = 50;
    speedTimerID = startTimer(speedInterval);

    powerDivisionNum = 6;
    minPower = -60;
    maxPower = 0;
    powerInterval = 10;
    for (int i = maxPower; i >= minPower; i-=powerInterval)
        powerLabelList << QString("%1").arg(i);

    minAngle = 0;
    maxAngle = 360;
    angleInterval = 30;
    for (int i = minAngle; i < maxAngle; i+=angleInterval)
        angleLabelList << QString("%1").arg(i);
}

PDisplay::~PDisplay() {}

void PDisplay::paintEvent( QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::TextAntialiasing);

    w = width();
    h = height();
    cx = w/2;
    cy = h/2;
    radius = qMin(w,h);
    int r = radius;//*cylinderNum*1.0/(cylinderNum+1.0);
    if (w > h)
        drawArea = QRect((w-h)/2, 0, r, r);
    else
        drawArea = QRect(0, (h-w)/2, r, r);

    drawGrid(&painter);
    drawLabel(&painter);
    drawAxisLabel(&painter);
    drawScan(&painter);
    drawPoints(&painter);
}

void PDisplay::drawGrid(QPainter *painter)
{
    painter->save();

    ////////////grid///////////////////////////////////////
    QColor color = Qt::darkGreen;//QColor(0, 100, 0);
    QPen pen = QPen(color, 1);

    int count = powerDivisionNum;
    int step = radius/(count+1);  // add 1 and leave space for label
    int x = w*0.2;
    int y = h*0.2;
    int r;

    QPainterPath looppath;
    QRectF outrect;
    for (int i = 1; i <= count; ++i)
    {
        r = step*i;
        x = cx-r/2;
        y = cy-r/2;
        QRectF rect(x,y,r,r);
        looppath.addEllipse(rect);
        outrect = rect;
    }

    // max out rect fill with white background
    painter->setPen(pen);
    painter->setBrush(Qt::white);
    painter->drawEllipse(outrect);
    painter->drawPath(looppath);
    if (angleInterval == 0)
        angleInterval =90;
    int linecount = 360/angleInterval;
    int newradius = outrect.height()/2;
    for (int i = 0 ; i < linecount; ++i)
    {
        x = cx + newradius*qSin(M_PI*2/linecount*i);
        y = cy + newradius*qCos(M_PI*2/linecount*i);

        painter->drawLine(cx,cy,x,y);
    }

    painter->restore();
}

void PDisplay::drawScan(QPainter *painter)
{
    painter->save();

    QColor color = Qt::green;
    QConicalGradient conicalGradient(drawArea.center(), pieAngle);
    color.setAlpha(100);
    conicalGradient.setColorAt(0, color);
    color.setAlpha(20);
    conicalGradient.setColorAt(0.1, color);
    color.setAlpha(0);
    conicalGradient.setColorAt(1, color);

    painter->setPen(Qt::NoPen);
    painter->setBrush(conicalGradient);

    painter->drawPie(drawArea, pieAngle * 16, 360 * 16);

    painter->restore();
}

void PDisplay::drawAxisLabel( QPainter *painter )
{
	painter->save();

    int laycount = powerDivisionNum;
    int r = radius/2;

	QFont font;
	int fontsize = 10;

	font.setPointSize(fontsize);
	painter->setFont(font);
    QPen pen(Qt::gray);
    painter->setPen(pen);
	for (int j = 0 ; j < laycount+1; ++j)
	{
        int nr = r*j/(laycount+1);
        int x = cx + nr*qSin(M_PI);
        int y = cy + nr*qCos(M_PI)+20;
        if (j < powerLabelList.count())
            painter->drawText(x,y,powerLabelList.at(j));
	}

	painter->restore();
}

void PDisplay::drawLabel( QPainter *painter )
{
	painter->save();

    int count = angleLabelList.length();

	QFont font;
	int fontsize = 10;
    font.setPointSize(fontsize);
	painter->setFont(font);
    QPen pen(Qt::gray);
    painter->setPen(pen);

    QFontMetrics fm(font);
    int pw, ph;
    QString s;
    int nr = radius/2*powerDivisionNum/(powerDivisionNum+1);
    for (int i = 0; i < count; ++i)
    {
        s = angleLabelList.at(i);
//        pw = fm.width(s);
        pw = fm.horizontalAdvance(s);
        ph = fm.height();
        int x, y;

        if (s == "0") {
            x = cx - pw/2;
            y = cy - nr - ph/4;
        } else if (s == "90") {
            x = cx + nr + ph/4;
            y = cy + ph/4;
        } else if (s == "180") {
            x = cx - pw/2;
            y = cy + nr + ph;
        } else if (s == "270") {
            x = cx - nr - pw - ph/4;
            y = cy + ph/4;
        } else {

            x = cx + nr*qSin(M_PI*2/count*i);
            y = cy - nr*qCos(M_PI*2/count*i);

            if (y < cy && x > cx) {
                y -= ph/4;
            }
            else if (y > cy && x > cx) {
                y += ph;
            }
            else if (y > cy && x < cx) {
                x -= pw;
                y += ph;
            }
            else if (y < cy && x < cx) {
                x -= pw;
                y -= ph/4;
            }
        }

        painter->drawText(x, y, s);
    }

	painter->restore();
}

void PDisplay::drawPoints( QPainter *painter )
{
    int r = radius/2*powerDivisionNum/(powerDivisionNum+1);
    QPen pen = QPen(Qt::red, 1);
    qreal step = 1.0*r/(maxPower-minPower);

    painter->save();

    painter->setPen(pen);
    painter->setBrush(Qt::red);
    for(int i=0; i<targetInfoList.length(); i++)
    {
        int id = targetInfoList[i].ID;
        float azimuth = targetInfoList[i].azimuth;
        float power = targetInfoList[i].power;

        qreal length = step*qAbs(power);
        qreal angle = (90-azimuth)*M_PI/180; // rotate 90
        qreal x = cx + length*qCos(angle);
        qreal y = cy - length*qSin(angle);

        painter->drawEllipse(QPointF(x, y), 12, 12);

        QFont font;
        int fontsize = 8;
        font.setPointSize(fontsize);
        painter->setFont(font);
        QPen pen(Qt::red);
        painter->setPen(pen);

        QString s = QString::number(id) + ":" +
                QString::number(azimuth, 'f',2) + ":" +
                QString::number(power, 'f',2);
        QFontMetrics fm(font);
        int pw = fm.horizontalAdvance(s);
        int ph = fm.height();
        painter->drawText(x-pw/2, y-ph, s);
    }

	painter->restore();
}

void PDisplay::timerEvent(QTimerEvent *event)
{
    pieAngle -= 2;
    update();
}

void PDisplay::setTargetInfo(QList<TargetInfo> &list)
{
    targetInfoList = list;
    update();
}
