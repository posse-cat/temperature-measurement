#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "curvepattern.h"
#include "tcpnode.h"
#include "tcpconfwindow.h"
#include "fiberconfwindow.h"
#include "cloudmap.h"


#include <QFont>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    this->setWindowTitle("碳纤维臂架温度实时监测系统");

    //默认一开始同时收看四条通道的温度情况
    ui->checkBox_1->setChecked(true);
    ui->checkBox_2->setChecked(true);
    ui->checkBox_3->setChecked(true);
    ui->checkBox_4->setChecked(true);

    //创建tcp通讯的对象
    tcp_node = new TcpNode();

    // 开始时 tcp 连接状态一定是断开的
    this->setLED(ui->label_led,1,this->radius);
    this->setConnectStateDisp(ui->label_state,0);

    //更新连接状态
    QObject::connect(tcp_node,SIGNAL(updateConnectState(bool)),this,SLOT(updateTcpConnectionState(bool)));

    //复选框状态改变
    QObject::connect(ui->checkBox_1,&QCheckBox::stateChanged,this,&MainWindow::updateCurveDisplay);
    QObject::connect(ui->checkBox_2,&QCheckBox::stateChanged,this,&MainWindow::updateCurveDisplay);
    QObject::connect(ui->checkBox_3,&QCheckBox::stateChanged,this,&MainWindow::updateCurveDisplay);
    QObject::connect(ui->checkBox_4,&QCheckBox::stateChanged,this,&MainWindow::updateCurveDisplay);

    //创建曲线实例
    createCurveObject();

    //初始状态为总览
    ui->stackedWidget->setCurrentIndex(0);

    //总览所有通道的曲线
    setTotalChart();

    /*  当堆栈窗口的页面发生变化时候，将QChart对象设置在对应的窗口中
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged, [&](int index) {
        qDebug() << "当前子窗口索引已更改：" << index;
    });
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged,this,&MainWindow::assignQChartReuse);
    以上是想在不同窗口页面复用同一个QChart对象（没成功），现在直接为每一个都创建了，没必要这么做了*/

    //更新曲线信息
    QObject::connect(tcp_node, &TcpNode::updateCurve,this,&MainWindow::drawCurveOnBox);

    //左下角区域显示
    ui->listView->setModel(&message_list);
    QObject::connect(tcp_node,&TcpNode::tcpnodeShow,this,&MainWindow::showMessage);

    //温度云图
    initCloudMap();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tcp_node;
}

int MainWindow::compute_bool2int(bool state)
{
    if (state)
    {
        return 1;
    }
    return 0;
}

void MainWindow::setLED(QLabel *label, int color, int size)
{
    label->setText("");
    QString min_width = QString("min-width: %1px;").arg(size);
    QString min_height = QString("min-height: %1px;").arg(size);
    QString max_width = QString("max-width: %1px;").arg(size);
    QString max_height = QString("max-height: %1px;").arg(size);

    QString border_radius = QString("border-radius: %1px;").arg(size/2);
    QString border = QString("border:1px solid black;");

    QString background = "background-color:";
    switch (color) {
    case 0:
        //灰色
        background += "rgb(190, 190, 190)";
        break;
    case 1:
        //红色
        background += "rgb(255, 0, 0)";
        break;
    case 2:
        //绿色
        background += "rgb(0, 255, 0)";
        break;
    default:
        break;
    }
    const QString Sheetstyle1 = min_width+min_height+max_width+max_height +border_radius+border+background;
    label->setStyleSheet(Sheetstyle1);
}

void MainWindow::setConnectStateDisp(QLabel *label, int mode)
{
    //设置字体
    QFont font("Arial", 12); // 使用Arial字体，字号为16
    font.setBold(true); // 可以设置字体加粗
    font.setItalic(true); // 可以设置字体斜体
    // 将字体应用到QLabel上
    label->setFont(font);

    switch (mode) {
    case 0 :
        label->setText("设备连接断开 !");
        break;
    case 1:
        label->setText("设备已连接...");
        break;
    }
}

void MainWindow::createCurveObject()
{
    //创建曲线实例
    this->curve_ch1_t = QSharedPointer<CurvePattern>(new CurvePattern());
    this->curve_ch2_t = QSharedPointer<CurvePattern>(new CurvePattern());
    this->curve_ch3_t = QSharedPointer<CurvePattern>(new CurvePattern());
    this->curve_ch4_t = QSharedPointer<CurvePattern>(new CurvePattern());

    this->curve_ch1_l = QSharedPointer<CurvePattern>(new CurvePattern());
    this->curve_ch2_l = QSharedPointer<CurvePattern>(new CurvePattern());
    this->curve_ch3_l = QSharedPointer<CurvePattern>(new CurvePattern());
    this->curve_ch4_l = QSharedPointer<CurvePattern>(new CurvePattern());
}

void MainWindow::setTotalChart()
{
    // set chart in  stack_widget 0
    curve_ch1_t->get_m_chart()->setTitle("通道1");
    curve_ch1_t->get_m_lineSeries()->setPointsVisible(false);
    ui->graphicsView_t1->setChart(curve_ch1_t->get_m_chart());
    ui->graphicsView_t1->setRenderHint(QPainter::Antialiasing);
    curve_ch2_t->get_m_chart()->setTitle("通道2");
    ui->graphicsView_t2->setChart(curve_ch2_t->get_m_chart());
    ui->graphicsView_t2->setRenderHint(QPainter::Antialiasing);
    curve_ch3_t->get_m_chart()->setTitle("通道3");
    ui->graphicsView_t3->setChart(curve_ch3_t->get_m_chart());
    ui->graphicsView_t3->setRenderHint(QPainter::Antialiasing);
    curve_ch4_t->get_m_chart()->setTitle("通道4");
    ui->graphicsView_t4->setChart(curve_ch4_t->get_m_chart());
    ui->graphicsView_t4->setRenderHint(QPainter::Antialiasing);

    // set chart in  stack_widget 1\2\3\4
    curve_ch1_l->get_m_chart()->setTitle("通道1");
    ui->graphicsView_1->setChart(curve_ch1_l->get_m_chart());
    ui->graphicsView_1->setRenderHint(QPainter::Antialiasing);
    curve_ch2_l->get_m_chart()->setTitle("通道2");
    ui->graphicsView_2->setChart(curve_ch2_l->get_m_chart());
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
    curve_ch3_l->get_m_chart()->setTitle("通道3");
    ui->graphicsView_3->setChart(curve_ch3_l->get_m_chart());
    ui->graphicsView_4->setRenderHint(QPainter::Antialiasing);
    curve_ch4_l->get_m_chart()->setTitle("通道4");
    ui->graphicsView_4->setChart(curve_ch4_l->get_m_chart());
    ui->graphicsView_4->setRenderHint(QPainter::Antialiasing);
}

// 重写 paintEvent() 函数来设置 QWidget 的背景图
/*
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(ui->widget1);
    QPixmap pixmap(":/toolbar/img_pkg/testArm2.png");
    painter.drawPixmap(0, 0, pixmap.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
*/

void MainWindow::initCloudMap()
{
    this->surface_map1=QSharedPointer<CloudMapWidget>(new CloudMapWidget(ui->cloudW1));
    //this->surface_map1=QSharedPointer<CloudMapWidget>(new CloudMapWidget(ui->cloudW1));
    //this->surface_map1=QSharedPointer<CloudMapWidget>(new CloudMapWidget(ui->cloudW1));
    //this->surface_map1=QSharedPointer<CloudMapWidget>(new CloudMapWidget(ui->cloudW1));
}

void MainWindow::on_action_connect_triggered()
{
    if(tcp_ui_ptr==nullptr)
    {
        tcp_window=new TcpConfWindow();
        tcp_ui_ptr=tcp_window;
        tcp_window->setAttribute(Qt::WA_DeleteOnClose);

        //配置子窗口的名称和显示位置
        tcp_window->setWindowTitle("配置 Modbus TCP 通信");
        // 假设 parentWidget 是父窗口的指针，childWidget 是子窗口的指针
        QRect parentRect = this->rect(); // 获取父窗口的矩形区域
        int centerX = parentRect.center().x(); // 计算父窗口的X中心点
        int centerY = parentRect.center().y(); // 计算父窗口的Y中心点
        tcp_window->move(centerX, centerY); // 将子窗口移动到父窗口的中心点
        tcp_window->show();
    }
    else
    {
        tcp_window=tcp_ui_ptr.data();
        tcp_window->hide();
        tcp_window->showNormal();
    }
    //tcp 通信配置
    // 注：原则上不需要调用disconnect，因为对象被释放时，会自动释放该对象的connect。
    // https://blog.csdn.net/yuyefuxiao521/article/details/113185099
    QObject::connect(tcp_window,SIGNAL(tcp_conf(QString,quint16)),tcp_node,SLOT(configure_tcp_connection(QString,quint16)));
}

void MainWindow::on_action_disconnect_triggered()
{
    tcp_node->setTcpConnectionBreak();
}

void MainWindow::on_action_calibration_triggered()
{

}

void MainWindow::on_pushButton_cr_list_pressed()
{
    ui->listView->model()->removeRows(0,ui->listView->model()->rowCount());
}

void MainWindow::updateTcpConnectionState(bool cstate)
{
    //qDebug()<<"Led reciver:"<<cstate;
    if(cstate)
    {
        this->setLED(ui->label_led,2,this->radius);
        this->setConnectStateDisp(ui->label_state,1);
    }
    else
    {
        this->setLED(ui->label_led,1,this->radius);
        this->setConnectStateDisp(ui->label_state,0);
    }
}

void MainWindow::updateCurveDisplay()
{
    int box_s1,box_s2,box_s3,box_s4;

    box_s1=this->compute_bool2int(ui->checkBox_1->isChecked());
    box_s2=this->compute_bool2int(ui->checkBox_2->isChecked());
    box_s3=this->compute_bool2int(ui->checkBox_3->isChecked());
    box_s4=this->compute_bool2int(ui->checkBox_4->isChecked());
    //qDebug()<<box_s1+box_s2+box_s3+box_s4;

    if ((box_s1+box_s2+box_s3+box_s4)==1)
    {
        if(box_s1==1) {
            ui->stackedWidget->setCurrentIndex(1);
        }
        else if (box_s2==1) {
            ui->stackedWidget->setCurrentIndex(2);
        }
        else if (box_s3) {
            ui->stackedWidget->setCurrentIndex(3);
        }
        else if (box_s4) {
            ui->stackedWidget->setCurrentIndex(4);
        }
    }

    if ((box_s1+box_s2+box_s3+box_s4)==4)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }

}

void MainWindow::drawCurveOnBox()
{
    /*
     *
        &&&&&&&&&&&&&&&    测试段落    &&&&&&&&&&&&&&&&&
     *
    QString telegram;
    QFile file("/home/pp/temper-ui-v2/testFile/record.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        qDebug()<< str;
        telegram.append(str);
    }

    curve_ch1_t->get_m_lineSeries()->clear();

    for (int var=0;var<180;var+=2)
    {
        int h1,h2,l1,l2;
        h1=(23+var)*3;
        h2=h1+1;
        l1=(22+var)*3;
        l2=l1+1;
        QString tem_HexCode=QString("%1%2%3%4").arg(telegram[h1]).arg(telegram[h2]).arg(telegram[l1]).arg(telegram[l2]);
        //qDebug()<<tem_HexCode;
        bool ok_status;
        int tem=tem_HexCode.toInt(&ok_status,16);
        double tem_=(tem-20000)/100.;

        int count=var/2;
        qDebug()<<tem_;
        curve_ch1_t->get_m_lineSeries()->append((QPointF(count, tem_)));
        curve_ch1_l->get_m_lineSeries()->append((QPointF(count, tem_)));
    }
    */

    int node_number_start=92*2*2+2;
    int node_number_end=112*2*2;
    int distance=1;
    bool ok_status;
    QString* data_ptr= tcp_node->getTelegram();
    //qDebug()<<data_ptr->at(4);

    //先清除前一次的曲线
    curve_ch1_t->get_m_lineSeries()->clear();
    curve_ch1_l->get_m_lineSeries()->clear();
    //清除前一次分布的云图
    //surface_map1->clearMapData();

    //40个节点分在3m臂架上
    double physicalStep = 300./40;

    //
    for (int var=node_number_start;var<node_number_end;var+=2)
    {
        int h1,h2,l1,l2;
        h1=(23+var)*2;
        h2=h1+1;
        l1=(22+var)*2;
        l2=l1+1;
        QString tem_HexCode=QString("%1%2%3%4").arg(data_ptr->at(h1)).arg(data_ptr->at(h2)).arg(data_ptr->at(l1)).arg(data_ptr->at(l2));
        //qDebug()<<tem_HexCode;
        int tem=tem_HexCode.toInt(&ok_status,16);
        double tem_=(tem-20000)/100.;
        distance++;
        //qDebug()<<tem_;
        // drawing in QChartView
        curve_ch1_t->get_m_lineSeries()->append((QPointF(distance, tem_)));
        curve_ch1_l->get_m_lineSeries()->append((QPointF(distance, tem_)));
        // drawing in QCustomPlot
        surface_map1->allocateTemperaturePoint(distance*physicalStep,tem_);
    }
    surface_map1->get_qcp_ptr()->replot();
}

void MainWindow::showMessage(QString msg)
{
    message_list.insertRows(message_list.rowCount(),1);
    QVariant new_row(msg);
    message_list.setData(message_list.index(message_list.rowCount()-1),new_row);
    ui->listView->scrollToBottom();
}
