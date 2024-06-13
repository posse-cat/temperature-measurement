#ifndef CLOUDMAP_H
#define CLOUDMAP_H
#include <QWidget>

class QCustomPlot;
class QCPColorMap;
class QCPColorScale;

class CloudMapWidget:public QWidget
{
public:
    CloudMapWidget(QCustomPlot *);
    virtual ~CloudMapWidget();

    void initMap(int,int,int,int);
    void allocateTemperaturePoint(double,double);
    void clearMapData();
    //void setCurvePoint();

    QCustomPlot * get_qcp_ptr(){return this->qcp_ptr;}

private:
    QCustomPlot * qcp_ptr;
    QCPColorMap * m_colorMap;
    QCPColorScale * m_colorScale;
};

#endif // CLOUDMAP_H
