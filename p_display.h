#ifndef P_DISPLAY_H
#define P_DISPLAY_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMap>

typedef struct {
    unsigned int ID;
    float azimuth;
    float power;
    float elevation;
    float frequency;
    float bandwidth;
}TargetInfo;

class PDisplay : public QWidget
{
	Q_OBJECT

public:
    PDisplay(QWidget *parent = 0);
    ~PDisplay();

protected:
	void paintEvent(QPaintEvent *e);
    void timerEvent(QTimerEvent *event);

private:
	void drawAxisLabel(QPainter *painter);
	void drawLabel(QPainter *painter);
    void drawPoints(QPainter *painter);
    void drawScan(QPainter *painter);
    void drawGrid(QPainter *painter);

private:
    int maxPower, minPower;
    int powerInterval;
    QStringList powerLabelList; // amplitude
    int powerDivisionNum;

    int maxAngle, minAngle;
    int angleInterval;    // angle interval 30
    QStringList angleLabelList;     // angle label list

    //QMap<qreal, qreal> targetInfo;  //angle, amplitude
    QList<TargetInfo> targetInfoList;  //angle, amplitude
    int radius;
    qreal pieAngle;
    int speedTimerID;
    int speedInterval;
    QRect drawArea;
    int w, h;
    int cx, cy;

public slots:
    void setTargetInfo(QList<TargetInfo> &list);
};

#endif
