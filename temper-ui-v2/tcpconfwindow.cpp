#include "tcpconfwindow.h"
#include "ui_tcpconfwindow.h"
#include <QDebug>

TcpConfWindow::TcpConfWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpConfWindow)
{
    ui->setupUi(this);

    //设备的默认ip和端口号
    ui->lineEdit_ip->setText("192.168.1.15");
    ui->lineEdit_port->setText("5000");

}

TcpConfWindow::~TcpConfWindow()
{
    delete this->ui;
}

void TcpConfWindow::on_pushButton_ok_pressed()
{
    QString tcp_ip = ui->lineEdit_ip->text();

    bool isOk;
    quint16 port_number = 0;
    port_number = ui->lineEdit_port->text().toUShort(&isOk);   // isOk可以用来判断 QString转成quint16是否转换成功
    //qDebug()<<"isOk:"<<isOk;
    if (isOk){emit tcp_conf(tcp_ip,port_number);}
    //this->close();
}

void TcpConfWindow::on_pushButton_return_pressed()
{
    this->close();
}


