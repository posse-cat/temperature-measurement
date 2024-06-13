#ifndef TCPNODE_H
#define TCPNODE_H
#include <QThread>
#include <QTime>
#include <QTcpSocket>

class TcpNode : public QObject
{
    Q_OBJECT
public:
    TcpNode();
    virtual ~TcpNode();
    bool getTcpConnectionState();
    void setTcpConnectionBreak();
    void connection_already_build();

    void Delay_MSec_Suspend(unsigned int);

    QString * getTelegram(){return telegram_hex_ptr;}

signals:
    void updateConnectState(bool);
    void updateCurve();
    void tcpnodeShow(QString);

public Q_SLOTS:
    void configure_tcp_connection(QString,quint16);
    void collectData();

private:
    QTcpSocket* socket_ptr;
    QString *telegram_hex_ptr;
};


#endif // TCPNODE_H
