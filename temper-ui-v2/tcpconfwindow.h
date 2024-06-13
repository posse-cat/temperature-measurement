#ifndef TCPCONFWINDOW_H
#define TCPCONFWINDOW_H

#include <QMainWindow>

namespace Ui {
class TcpConfWindow;
}

class TcpConfWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpConfWindow(QWidget *parent = nullptr);
    ~TcpConfWindow();

signals:
    void tcp_conf(QString,quint16);

public Q_SLOTS:
    void on_pushButton_ok_pressed();
    void on_pushButton_return_pressed();

private:
    Ui::TcpConfWindow *ui;

};

#endif // TCPCONFWINDOW_H
