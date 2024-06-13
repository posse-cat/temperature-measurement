#include "tcpnode.h"
#include <QDebug>

TcpNode::TcpNode()
{
    socket_ptr = new QTcpSocket();
    telegram_hex_ptr=new QString;

    QObject::connect(socket_ptr, &QTcpSocket::readyRead, this, &TcpNode::collectData);
    QObject::connect(socket_ptr, &QAbstractSocket::connected,this, &TcpNode::connection_already_build);
}

TcpNode::~TcpNode()
{
    delete socket_ptr;
    delete telegram_hex_ptr;
}

bool TcpNode::getTcpConnectionState()
{
    return false;
}

void TcpNode::setTcpConnectionBreak()
{
    //socket_ptr->disconnectFromHost();
    socket_ptr->abort();
    emit updateConnectState(false);
    emit tcpnodeShow("disconnect action complete!!");
    }

void TcpNode::connection_already_build()
{
    emit updateConnectState(true);
    emit tcpnodeShow("TCP connect success!!");
}

void TcpNode::Delay_MSec_Suspend(unsigned int msec)
{

    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer );
}

void TcpNode::configure_tcp_connection(QString ip,quint16 port)
{ 
    socket_ptr->connectToHost(ip,port);



    //Delay_MSec_Suspend(1000);

    if (socket_ptr->state()==QAbstractSocket::ConnectedState)
    {
        //已连接
        emit tcpnodeShow("successfully...");
    }
    else if (socket_ptr->state() == QAbstractSocket::ConnectingState)
    {
        //正在连接
        //  第一次进行连接之后，socket会返回这个状态，然后连接上时会发送 &readyRead信号
        //  只有连完后再次查看 state时候，才会得到 已连接 的状态
        emit tcpnodeShow("connecting...");

    } else if (socket_ptr->state() == QAbstractSocket::UnconnectedState)
    {
        emit tcpnodeShow("disconnected...");
        // 未连接
    } else
    {
        emit tcpnodeShow("Unknow Case...");// 其他情况
    }

    //
    //qDebug()<<"tcp_ip:"<<ip;
    //qDebug()<<"port_number:"<<port;
    //
    QString msg1="ip:"+ip;
    QString msg2="port:"+QString::number(port);
    emit tcpnodeShow(msg1);
    emit tcpnodeShow(msg2);
}

void TcpNode::collectData()
{
    emit tcpnodeShow("recieve a curve..");
    //这个里面存储编码不太认识，有的是16进制转义码，中间又穿插了字符ascii码
    QByteArray telegram_data=socket_ptr->readAll();
    //直接无脑转了
    *telegram_hex_ptr=telegram_data.toHex();
    //qDebug()<<*telegram_hex_ptr;
    emit updateCurve();
}
