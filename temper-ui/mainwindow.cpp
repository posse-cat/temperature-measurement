#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtGui>

//scan the serial-port
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    spnode()
{
    ui->setupUi(this);
    initComponent();
    pollTimer = new QTimer;
    //temper_queue_ptr=new std::queue<int>;

    //设置底部图标
    QApplication::setWindowIcon(QIcon(":/ui_pattern/img_pkg/thermometer.jpg"));

    //左下角窗口信息输出
    ui->listView_12->setModel(spnode.loggingModel());
    QObject::connect(&spnode,SIGNAL(update_listview()),this,SLOT(showListMessage()));

    //设置“关闭串口”的按钮不可选用，因为QSerialPort为空时，
    // 执行其成员函数会使整个页面关闭。（不知道什么原因，也不想知道，艹）
    // 早期程序中存在空指针，空指针直接调用函数
    ui->pushButton_16b->setEnabled(false);

    //LCD显示器
    QObject::connect(&spnode,SIGNAL(update_lcd_display(quint16)),this,SLOT(showTemperInLcd(quint16)));

    //子窗口
    QObject::connect(this,SIGNAL(curve1(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve2(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve3(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve4(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve5(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve6(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve7(QString)),this,SLOT(showSubUI(QString)));
    QObject::connect(this,SIGNAL(curve8(QString)),this,SLOT(showSubUI(QString)));

    //连续读取温度
    QObject::connect(pollTimer,SIGNAL(timeout()), this, SLOT(on_action_once_triggered()));

}

MainWindow::~MainWindow()
{
    delete ui;
    //delete independent_ui;
    //delete pollTimer;
    //m_ptr=nullptr;
}

void MainWindow::initComponent()
{
    //串口配置参数
    QStringList nameList;//串口号
    QStringList baudList;//波特率
    QStringList parityList;//校验位
    QStringList dataBitsList; //数据位
    QStringList stopBitsList; //停止位
    //串口配置参数选项
    nameList<<"";
    baudList<<"4800"<<"9600"<<"19200";
    parityList<<"无"<<"奇"<<"偶"<<"空格";
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    stopBitsList<<"1"<<"1.5"<<"2";
    //将置入下拉栏中
    ui->comboBox_1->addItems(nameList);
    ui->comboBox_2->addItems(baudList);
    ui->comboBox_3->addItems(parityList);
    ui->comboBox_4->addItems(dataBitsList);
    ui->comboBox_5->addItems(stopBitsList);

    //默认配置
    ui->comboBox_2->setCurrentIndex(1);//波特率: 9600
    ui->comboBox_3->setCurrentIndex(0);//校验位: 无
    ui->comboBox_4->setCurrentIndex(3);//数据位: 8
    ui->comboBox_5->setCurrentIndex(0);//停止位: 1

}

void MainWindow::offAndOn(bool status)
{

    ui->comboBox_1->setEnabled(status);
    ui->comboBox_2->setEnabled(status);
    ui->comboBox_3->setEnabled(status);
    ui->comboBox_4 ->setEnabled(status);
    ui->comboBox_5 ->setEnabled(status);
    //status false不可选   true可选
}

void MainWindow::showNoMessage() {
    // %QMessageBox% user manual ->
    // https://blog.csdn.net/kenfan1647/article/details/122922588
    QMessageBox msgBox;
    msgBox.setText("Please set the port name.");
    msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Ok);
    int rec = msgBox.exec();
    switch(rec)
    {
    case QMessageBox::Ok:
        break;
    case QMessageBox::Close:
        // quit Main-Window interface
        //close();
        break;
    }
    return;
}

void MainWindow::storeTemper(QString data)
{
    if (this->time_point>300){pollTimer->stop();}
    /*
    if (this->temper_queue_ptr->size()<=1800) //30分钟内的数据
    {
        this->temper_queue_ptr->push(current_v);
    }
    else
    {
        this->temper_queue_ptr->pop();
        this->temper_queue_ptr->push(current_v);
    }
    */
    /*获取当前上位机程序所在的路径，+filename是为了直接把文件名加进去 */
    QString filename = "temperature.txt";
    QString file_path = QCoreApplication::applicationDirPath() + "/" + filename;
    QFile file(file_path);
    /*
    //由于file.open函数的参数是char*类型  所以要将Qstring类型转一下
    char* str;
    QByteArray ch = data.toLatin1(); //文件编码格式转换
    str = ch.data();
    //设定文件的打开方式QIODevice::Append(追加)  如果当前文件下有这个文件就会追加，没有就会创建文件写入
    file.open(QIODevice::Append | QIODevice::Text);
    //向文件中写入内容
    file.write(str);

    file.close();
    */
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream textStream(&file);
    textStream << data << "\n";
    file.close();
}

void MainWindow::on_pushButton_refresh_pressed()
{
    //$$testcode$$
    /*
     * 马的，这个套方法很多博客都这样写的的，结果根本毛用没有
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        //这里只是临时读取可用串口不要在堆区直接开辟空间
        QSerialPort port;
        port.setPort(info);
        //以可读可写的方式打开(临时打开，扫描可用串口)
        if(port.open(QIODevice::ReadWrite))
        {
            //将扫描到的可用串口写入到comboBox_1里
            ui->comboBox_1->addItem(port.portName());
            //关闭串口
            port.close();
        }
        else
        {
            ui->comboBox_1->addItem("6");
            //这里记得要去补充一个异常提示，在左下角窗口打印未扫到可用的串口
        }
    }
    */

    //先清空原来的名称
    ui->comboBox_1->clear();
    //读取可用的串口
    auto portsInfo= QSerialPortInfo::availablePorts();
    for (auto info : portsInfo)
    {
        qInfo()<<info.description()<<info.portName();
        ui->comboBox_1->addItem(info.portName());
    }
    spnode.log(spnode.scan_result,"Scanned Once!");
    return;
}


void MainWindow::on_pushButton_16a_pressed()
{
    //showNoMessage();
    //==================================================
    //如果已经打开了串口，则先关闭当前打开的，然后再执行开启
    //==================================================
    spnode.set_port_name(ui->comboBox_1->currentText());
    spnode.set_baud_rate(ui->comboBox_2->currentText());
    spnode.set_parity_bit(ui->comboBox_3->currentText());
    spnode.set_data_bit(ui->comboBox_4->currentText());
    spnode.set_stop_bit(ui->comboBox_5->currentText());
    if (ui->comboBox_1->currentText()=="")
    {
        spnode.portConfShow();
        showNoMessage();
        return;
    }
    if (spnode.openPort())
    {
        //锁定串口配置栏
        offAndOn(false);
        ui->pushButton_16a->setEnabled(false);
        ui->pushButton_16b->setEnabled(true);
        ui->pushButton_refresh->setEnabled(false);
    }
}

void MainWindow::on_pushButton_16b_pressed()
{
    //恢复串口配置框
    offAndOn(true);
    ui->pushButton_16a->setEnabled(true);
    ui->pushButton_16b->setEnabled(false);
    ui->pushButton_refresh->setEnabled(true);
    spnode.closePort();
}

void MainWindow::on_action_once_triggered()
{
    spnode.sendRequest();
    this->time_point+=1;
    return;
}

void MainWindow::on_action_continuous_triggered()
{
    pollTimer->start(1000);//1000ms 一次
}

void MainWindow::on_action_suspend_triggered()
{
    pollTimer->stop();
    return;
}

void MainWindow::on_action_clear_list_triggered()
{
    //https://blog.csdn.net/whj1120367347/article/details/134809802
    ui->listView_12->model()->removeRows(0,ui->listView_12->model()->rowCount());
    return;
}

void MainWindow::on_pushButton_21a_pressed()
{
    //spnode.send

    spnode.log(spnode.info_result,"");
    QString name_("Node1");
    emit curve1(name_);
}

void MainWindow::on_pushButton_22a_pressed()
{
    QString name_("Node2");
    emit curve2(name_);
}

void MainWindow::on_pushButton_23a_pressed()
{
    QString name_("Node3");
    emit curve3(name_);
}

void MainWindow::on_pushButton_24a_pressed()
{
    QString name_("Node4");
    emit curve4(name_);
}

void MainWindow::on_pushButton_41a_pressed()
{
    QString name_("Node5");
    emit curve5(name_);
}

void MainWindow::on_pushButton_42a_pressed()
{
    QString name_("Node6");
    emit curve6(name_);
}

void MainWindow::on_pushButton_43a_pressed()
{
    QString name_("Node7");
    emit curve7(name_);
}

void MainWindow::on_pushButton_44a_pressed()
{
    QString name_("Node8");
    emit curve8(name_);
}

void MainWindow::on_pushButton_21b_pressed()
{

}


void MainWindow::showListMessage()
{
    ui->listView_12->scrollToBottom();
    return;
}

void MainWindow::showTemperInLcd(quint16 original_v)
{
    float temper=static_cast<float>(original_v);
    temper=temper/10;

    QString data_str= QString::number(temper,'f',2);
    storeTemper(data_str);

    *c_temper_ptr=temper;

    double show_value=static_cast<double>(temper);
    ui->lcdNumber_21->display(show_value);
    ui->widget_31->changeValue(show_value);

}

void MainWindow::showSubUI(QString ui_name)
{
    if(m_ptr==nullptr)
    {
        independent_ui=new CurveWindow();
        m_ptr=independent_ui;
        independent_ui->setAttribute(Qt::WA_DeleteOnClose);

        independent_ui->setWindowTitle(ui_name);
        independent_ui->show();
    }
    else
    {
        independent_ui=m_ptr.data();

        independent_ui->hide();
        independent_ui->showNormal();
    }

    independent_ui->setTemperPtr(this->c_temper_ptr);

}
