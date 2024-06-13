#include "cloudmap.h"

#include "qcustomplot.h"

CloudMapWidget::CloudMapWidget(QCustomPlot *ui_widget_ptr)
{
    this->qcp_ptr= ui_widget_ptr;

    //
    initMap(300,-15,50,41);
}

CloudMapWidget::~CloudMapWidget()
{
    if (this->qcp_ptr)
    {
        delete qcp_ptr;
    }
}

void CloudMapWidget::initMap(int max_x,int min_t,int max_t,int particle_l)
{
    // configure axis rect:
    //qcp_ptr->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming

    // set the range of X/Y axis
    qcp_ptr->xAxis->setRange(0, max_x,Qt::AlignLeft);
    qcp_ptr->yAxis->setRange(0, 5,Qt::AlignLeft);
    qcp_ptr->axisRect()->setupFullAxesBox(true);

    //隐藏坐标，上下左右
    bool appearAxis=false;
    qcp_ptr->xAxis->setVisible(true);
    qcp_ptr->yAxis->setVisible(appearAxis);
    qcp_ptr->xAxis2->setVisible(appearAxis);
    qcp_ptr->yAxis2->setVisible(appearAxis);

    // set up the QCPColorMap:
    this->m_colorMap = new QCPColorMap(qcp_ptr->xAxis, qcp_ptr->yAxis);
    // add a color scale:
    this->m_colorScale = new QCPColorScale(qcp_ptr);
    qcp_ptr->plotLayout()->addElement(0, 1, m_colorScale); // add it to the right of the main axis rect
    m_colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    m_colorMap->setColorScale(m_colorScale); // associate the color map with the color scale
    // set the color gradient of the color map to one of the presets:
    m_colorMap->setGradient(QCPColorGradient::gpJet);
    // set range of data( Temperature )
    m_colorMap->setDataRange(QCPRange(min_t, max_t));
    //根据已知的少数温度点，插值生成整体的表面温度分布数据
    m_colorMap->setInterpolate(true);

    // 设置颜色图数据维度(云图的原始颗粒度)
    m_colorMap->data()->setSize(particle_l,5);
    // 颜色图在x、y轴上的区域
    m_colorMap->data()->setRange(QCPRange(0.05, max_x - 0.05), QCPRange(0.05, 5 - 0.05));

    // ( test code ) allocate temperature distribution
    //allocateTemperaturePoint(0.3,20);
    //allocateTemperaturePoint(0.7,20);
    //allocateTemperaturePoint(1.1,20);
    //allocateTemperaturePoint(1.2,30);
    //allocateTemperaturePoint(1.3,25);
    //allocateTemperaturePoint(1.4,40);
    //allocateTemperaturePoint(1.5,33);
    //allocateTemperaturePoint(1.6,33);
    //allocateTemperaturePoint(2.1,14);
    //allocateTemperaturePoint(2.2,42);
    //allocateTemperaturePoint(2.9,15);
}

void CloudMapWidget::allocateTemperaturePoint(double x,double t)
{
    //将云图中温度较低的区域设置一定的透明度，增加美观
    double tol=20.;
    if (t<tol)
    {
        m_colorMap->data()->setAlpha(int(x),0,228);
        m_colorMap->data()->setAlpha(int(x),1,218);
        m_colorMap->data()->setAlpha(int(x),2,178);
        m_colorMap->data()->setAlpha(int(x),3,178);
        m_colorMap->data()->setAlpha(int(x),4,218);
        m_colorMap->data()->setAlpha(int(x),5,228);
    }
    m_colorMap->data()->setData(x,0,t*0.68);
    m_colorMap->data()->setData(x,1,t*0.95);
    m_colorMap->data()->setData(x,2,t);
    m_colorMap->data()->setData(x,3,t);
    m_colorMap->data()->setData(x,4,t*0.95);
    m_colorMap->data()->setData(x,5,t*0.68);
}

void CloudMapWidget::clearMapData()
{
    m_colorMap->data()->clear();
    m_colorMap->data()->clearAlpha();
}


