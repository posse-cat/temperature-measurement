#ifndef CURVEPATTERN_H
#define CURVEPATTERN_H
#include <QThread>

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QTimer>

//光添加头文件还不够，需要声明charts的命名空间，方能生效
QT_CHARTS_USE_NAMESPACE

class CurvePattern: public QThread
{
    Q_OBJECT
public:
    CurvePattern();
    virtual ~CurvePattern();
    void init_Chart_Axis();
    void curveShow();

    QChart* get_m_chart() {return m_chart;}
    QValueAxis* get_m_axisY(){return m_axisY;}
    QValueAxis* get_m_axisX(){return m_axisX;}
    QLineSeries*get_m_lineSeries(){return m_lineSeries;}

public Q_SLOTS:
    void testSlotTimeout();

private:
    QChart *m_chart=nullptr;
    QValueAxis *m_axisY=nullptr;
    QValueAxis *m_axisX=nullptr;
    QLineSeries *m_lineSeries=nullptr;

    QTimer *chartTimer;

    //坐标轴数值跨度
    const int AXIS_MAX_X = 50, AXIS_MAX_Y = 50;

    //做图的时间坐标
    int count=0;
};

#endif // CURVEPATTERN_H
