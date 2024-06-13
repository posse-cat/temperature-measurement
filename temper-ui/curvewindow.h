#ifndef CURVEWINDOW_H
#define CURVEWINDOW_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QTimer>
//光添加头文件还不够，需要声明charts的命名空间，方能生效
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class CurveWindow;
}

class CurveWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CurveWindow(QWidget *parent = nullptr);
    ~CurveWindow();
    void setTemperPtr(float*const ptr){temper_ptr=ptr;}

public Q_SLOTS:
    void on_pushButton_return_pressed();

    void updateWidget();

protected:
    void initChart();

private:
    Ui::CurveWindow *ui;
    QChart *m_chart=nullptr;
    QValueAxis *m_axisY=nullptr;
    QValueAxis *m_axisX=nullptr;
    QLineSeries *m_lineSeries=nullptr;
    QTimer *chartTimer=nullptr;

    //坐标轴数值跨度
    const int AXIS_MAX_X = 60, AXIS_MAX_Y = 30;

    //
    int count=0;

    //当前温度
    float * temper_ptr;
};

#endif // CURVEWINDOW_H
