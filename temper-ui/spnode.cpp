#include "spnode.hpp"
#include <sstream>
#include <string>

SpNode::SpNode()
{
    mdbs_device_ptr=new QModbusRtuSerialMaster(this);
}

SpNode::~SpNode()
{
    delete mdbs_device_ptr;
}

//测试函数
void SpNode::portConfShow()
{
    log(info_result,"port name :"+this->port_name.toStdString());

    log(info_result,"baud rate :"+this->baud_rate.toStdString());

    log(info_result,"parity bit :"+this->parity_bit.toStdString());

    log(info_result,"data bit :"+this->data_bit.toStdString());

    log(info_result,"stop bit :"+this->stop_bit.toStdString());

    return;
}
/*
bool SpNode::openPort2()
{
    QSerialPort *serialPort = new QSerialPort;
    serialPort->setPortName("ttyUSB1"); // 根据你的系统修改端口名
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        qWarning() << "Cannot open serial port" << serialPort->errorString();
        return false;
    }

    // 构建Modbus请求
    // 假设我们要读取从机地址0x01，寄存器地址0x0000，读取1个寄存器
    QByteArray request;
    request.resize(5);
    request[0] = 0x01; // 从机地址
    request[1] = 0x03; // 功能代码 (读寄存器)
    request[2] = 0x00; // 寄存器地址高字节
    request[3] = 0x2e; // 寄存器地址低字节
    request[4] = 0x00; // 寄存器数量高字节
    request[5] = 0x01; // 寄存器数量低字节

    // 计算CRC
    quint16 crc = qChecksum(request.constData(), request.size());
    request.append((char)(crc & 0xFF)); // CRC低字节
    request.append((char)(crc >> 8));   // CRC高字节

    // 发送请求
    serialPort->write(request);

    // 读取响应
    while(serialPort->waitForReadyRead(1000))
    { // 等待响应，超时时间1000ms
        QByteArray response = serialPort->readAll();

        log(open_result,"here...");
        QString str;
        for(int i=0; i<response.size(); i++)
        {
            str += QString("%1").arg((unsigned char)response.at(i), 2, 16, QLatin1Char('0') ) + ' ';
        }
        log(info_result,str.toStdString());
    }

    return true;
}
*/
//打开串口
bool SpNode::openPort()
{
    if (!this->mdbs_device_ptr)
    {
        log(error_case,"mdbs_device_ptr==null ！");
        return false;
    }

    log(info_result,"openning..");

    //如果事先已经连接，先执行断开
    if (this->mdbs_device_ptr->state()==QModbusDevice::ConnectedState)
    {
        mdbs_device_ptr->disconnect();
    }
    //-----------------------基于modbus RTU协议---------------------------
    //端口名
    mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialPortNameParameter,this->port_name);

    //设置波特率
    if (this->baud_rate==tr("4800"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,QSerialPort::Baud4800);
    else if (this->baud_rate==tr("9600"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,QSerialPort::Baud9600);
    else if (this->baud_rate==tr("19200"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,QSerialPort::Baud19200);

    //校验位
    if(this->parity_bit == tr("无"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::NoParity);
    else if(this->parity_bit == tr("奇校验"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::OddParity);
    else if(this->parity_bit == tr("偶校验"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::EvenParity);
    else if(this->parity_bit == tr("空格"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::SpaceParity);

    //设置数据位
    switch (this->data_bit.toInt())
    {
    case 8:
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data8);
        break;
    case 7:
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data7);
        break;
    case 6:
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data6);
        break;
    case 5:
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data5);
        break;
    }

    //设置停止位
    if(this->stop_bit == tr("1"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,QSerialPort::OneStop);
    else if(this->stop_bit == tr("1.5"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,QSerialPort::OneAndHalfStop);
    else if(this->stop_bit == tr("2"))
        mdbs_device_ptr->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,QSerialPort::TwoStop);

    //设置响应超时
    mdbs_device_ptr->setTimeout(1000);
    mdbs_device_ptr->setNumberOfRetries(3);

    //连接从站设备并返回状态
    bool ok = mdbs_device_ptr->connectDevice();
    if(ok)
    {
        this->log(open_result,"&& UART open successfully &&");
        return true;
    }
    else
    {
        this->log(error_case,"&& fail to open UART &&");
        return false;
    }
}

//关闭串口
void SpNode::closePort()
{
    //断开modbus主从站之间的连接状态
    mdbs_device_ptr->disconnectDevice();
    //pollTimer->stop();

    log(open_result,"&&& UART closed &&&");
}

//发送读取寄存器请求（单次）  TX
void SpNode::sendRequest()
{
    //若中途关闭了串口，避免窗口线程崩溃
    // 每次发送前先确定一下 主从站 连接情况
    if(mdbs_device_ptr->state()==QModbusDevice::UnconnectedState)
    {
        log(error_case,"no device to send message！！");
        return;
    }

//-------------------------------------------------------------------
//                        配置 “读数据” 请求的报文
//-------------------------------------------------------------------
    //目前温度传感器就一个读取模式  >>  03 >> 保持寄存器
    QModbusDataUnit::RegisterType type;
    type=QModbusDataUnit::HoldingRegisters;

    //寄存器起始地址
    int start_address=40; // 0
    int channel_index=6;  // 0~11

    //test node
    int r_address=start_address+channel_index;

    //读取寄存器的数量
    quint16 numberOfEntries = 0x0001;

    //单条请求报文 03 00 2E 00 01
    QModbusDataUnit read_request_d(type,r_address,numberOfEntries);

    if(!read_request_d.isValid())
    {
        qDebug()<<"data is empty";
    }

    //log(info_result,);

    //发送报文   
    qint8 slave_station_index=1; //广播，或者也可以01
    QModbusReply *reply=mdbs_device_ptr->sendReadRequest(read_request_d,slave_station_index);

    if (!reply->isFinished())
    {
        //一般我们是要把连接操作放在构造函数中，避免函数多次调用时 重复连接信号与槽
        //但是，这里的对象会随函数结束而释放，因此不会出现重复的问题
        connect(reply, &QModbusReply::finished, this, &SpNode::parseData);
    }
    else
    {
        delete reply; // broadcast replies return immediately
    }
    /*
    if (auto *reply = mdbs_device_ptr->sendReadRequest(read_request_d, slave_station_index))
    {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &SpNode::parseData);
        else
            delete reply; // broadcast replies return immediately
    }
    else
    {
        log(error_case,mdbs_device_ptr->errorString().toStdString());
    }
    */
}


//接受下位机响应帧，解析报文  RX
void SpNode::parseData()
{
    auto reply=qobject_cast<QModbusReply*>(sender());

    if (!reply)
    {
        qDebug()<<"recive no data!!";
        return;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        quint16 temper = unit.value(0);
        emit(update_lcd_display(temper));
    }
    /*
    // 将下位机发来数据存储在数据缓冲区
    QByteArray arr;
    //转换成16进制显示
    arr=this->m_port->readAll().toHex();
    */
    //在List框中打印接收的数据
    //log(info_result,std::string("parse data..."));
}

void SpNode::log( const LogLevel &level, const std::string &msg) {
    message_list.insertRows(message_list.rowCount(),1);
    std::stringstream message_list_msg;
    switch ( level ) {
        case(scan_result) : {
          message_list_msg << "[scan->]: "<< msg;
          break;
        }
        case(open_result) : {
            message_list_msg << "[open->]: " << msg;
            break;
        }
        case(info_result) : {
            message_list_msg << "[data->]: " << msg;
            break;
        }
        case(error_case) : {
            message_list_msg << "[error->]: " << msg;
            break;
        }
    }
    QVariant new_row(QString(message_list_msg.str().c_str()));
    message_list.setData(message_list.index(message_list.rowCount()-1),new_row);
    Q_EMIT update_listview(); // used to readjust the scrollbar
} // function <log>



