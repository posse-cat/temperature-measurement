#include "curvewindow.h"
#include "ui_curvewindow.h"

CurveWindow::CurveWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurveWindow)
{
    ui->setupUi(this);
    chartTimer=new QTimer(this);

    initChart();

    QObject::connect(chartTimer,SIGNAL(timeout()),this,SLOT(updateWidget()));

    chartTimer->start(1000);
}

CurveWindow::~CurveWindow()
{
    delete ui;
}

void CurveWindow::on_pushButton_return_pressed()
{
    chartTimer->stop();
    this->close();
}

//实时更新曲线图
void CurveWindow::updateWidget()
{
    if(count > AXIS_MAX_X)
    {
        m_lineSeries->remove(0);
        m_chart->axisX()->setMin(count - AXIS_MAX_X);
        m_chart->axisX()->setMax(count); // 更新X轴范围
    }
    qreal c_temper=static_cast<qreal>(*temper_ptr);
    m_lineSeries->append(QPointF(count,c_temper)); // 更新显示
    count++;
}

void CurveWindow::initChart()
{
    if (m_chart==nullptr)
    {
        m_chart = new QChart();// 创建图表对象
    }

    if (m_lineSeries==nullptr)
    {
        m_lineSeries = new QLineSeries();// 创建曲线绘制对象
        m_lineSeries->setPointsVisible(true);// 设置数据点可见
        m_lineSeries->setName("温度曲线");// 图例名称
        m_chart->addSeries(m_lineSeries);
    }

    if (m_axisX==nullptr)
    {
        m_axisX = new QValueAxis();
        m_axisX->setTitleText("Time");
        m_axisX->setGridLineVisible(true);
    }

    if (m_axisY==nullptr)
    {
        m_axisY = new QValueAxis();
        m_axisY->setTitleText("Temperature");
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

    // 曲线对象关联上 坐标轴，此步骤必须在m_chart->addSeries之后
    m_lineSeries->attachAxis(m_axisX);
    m_lineSeries->attachAxis(m_axisY);

    //隐藏图例
    m_chart->legend()->hide();

    // 将图表对象设置到graphicsView上进行显示
    ui->graphicsView->setChart(m_chart);
    // 设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}
