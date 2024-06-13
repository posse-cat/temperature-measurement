#ifndef SPNODE_HPP_
#define SPNODE_HPP_

#include <iostream>
#include <string>
#include <QThread>
#include <QStringListModel>

//the serial-port
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

//modbus RTU
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QModbusClient>

//Timer
#include <QTime>


//class
class SpNode:public QThread{
    Q_OBJECT
public:
    SpNode();
    virtual ~SpNode();

    //test function
    void portConfShow();

    //function about serial communication
    void scanPortInfo();
    bool openPort();
    bool openPort2();
    void closePort();
    void sendRequest();
    void parseData();

    //消息类型
    enum LogLevel{
        scan_result,
        open_result,
        info_result,
        error_case,
    };
    //打印消息
    QStringListModel* loggingModel() { return &message_list;}
    void log(const LogLevel &level, const std::string &msg);

    //interface for uart parameter
    void set_port_name(QString v){this->port_name=v;}
    void set_baud_rate(QString v){this->baud_rate=v;}
    void set_parity_bit(QString v){this->parity_bit=v;}
    void set_data_bit(QString v){this->data_bit=v;}
    void set_stop_bit(QString v){this->stop_bit=v;}
    //
Q_SIGNALS:
    void update_listview();
    void update_lcd_display(quint16);

private:
    //串口对象
    //QSerialPort *m_port;

    //Modbus RTU协议对象
    QModbusReply *lastRequest = nullptr;
    QModbusClient * mdbs_device_ptr = nullptr;
    //用于定时发送功能

    //串口打印
    QStringListModel message_list;

    //串口配置参数
    QString port_name;
    QString baud_rate;
    QString parity_bit;
    QString data_bit;
    QString stop_bit;

};

#endif/* SPNODE_HPP_ */
