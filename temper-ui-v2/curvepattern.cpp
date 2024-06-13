#include "curvepattern.h"

CurvePattern::CurvePattern()
{
    //创建一个定时器
    chartTimer=new QTimer(this);

    //QObject::connect(chartTimer,SIGNAL(timeout()),this,SLOT(testSlotTimeout()));
    //chartTimer->start(1000);
    init_Chart_Axis();
}

CurvePattern::~CurvePattern()
{
    delete m_chart;
    delete m_axisX;
    delete m_axisY;
    delete m_lineSeries;
}

void CurvePattern::init_Chart_Axis()
{
    if (m_chart==nullptr)
    {
        m_chart = new QChart();// 创建图表对象
        //m_chart->setTitle();
    }

    if (m_axisX==nullptr)
    {
        m_axisX = new QValueAxis();
        //if (mode==1){m_axisX->setTitleText("位置");}
        m_axisX->setGridLineVisible(true);
    }

    if (m_axisY==nullptr)
    {
        m_axisY = new QValueAxis();
        //if (mode==1){m_axisY->setTitleText("温度");}
        m_axisY->setGridLineVisible(true);
    }

    //设置初始坐标轴范围
    m_axisX->setMin(0);
    m_axisY->setMax(0);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

    // 将坐标轴添加到图表上
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    if (m_lineSeries==nullptr)
    {
        m_lineSeries = new QLineSeries();// 创建曲线绘制对象
        m_lineSeries->setPointsVisible(true);// 设置数据点可见
        //m_lineSeries->setName(curve_name);// 图例名称
        m_chart->addSeries(m_lineSeries);
    }

    // 曲线对象关联上 坐标轴，此步骤必须在m_chart->addSeries之后
    m_lineSeries->attachAxis(m_axisX);
    m_lineSeries->attachAxis(m_axisY);

    //隐藏图例
    m_chart->legend()->hide();
}

void CurvePattern::curveShow()
{
    m_chart->show();
}

void CurvePattern::testSlotTimeout()
{
    if (m_lineSeries==nullptr)
    {
        return;
    }
    int count = m_lineSeries->points().size();                  // 获得当前数据序列点数
    if(count > AXIS_MAX_X)
    {
        m_chart->axisX()->setMax(count);                        // 更新X轴范围
    }
    m_lineSeries->append(QPointF(count, 20+rand() % 5));
}



