#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <queue>

#include "spnode.hpp"
#include "curvewindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initComponent();
    void offAndOn(bool);
    void showNoMessage();
    void storeTemper(QString);

signals:
    void curve1(QString);
    void curve2(QString);
    void curve3(QString);
    void curve4(QString);
    void curve5(QString);
    void curve6(QString);
    void curve7(QString);
    void curve8(QString);//显示每个节点的历史温度数据

public Q_SLOTS:
    //auto-connnetive slots function
    void on_pushButton_refresh_pressed();//“扫描串口”按钮
    void on_pushButton_16a_pressed();//“打开串口”按钮
    void on_pushButton_16b_pressed();//“关闭串口”按钮

    //顶部按钮
    void on_action_once_triggered(); //触发单次采集
    void on_action_continuous_triggered(); //开启连续采集
    void on_action_suspend_triggered();// 暂停连续采集
    void on_action_clear_list_triggered(); //清空消息栏


    //打开温度曲线弹窗
    void on_pushButton_21a_pressed();
    void on_pushButton_22a_pressed();
    void on_pushButton_23a_pressed();
    void on_pushButton_24a_pressed();
    void on_pushButton_41a_pressed();
    void on_pushButton_42a_pressed();
    void on_pushButton_43a_pressed();
    void on_pushButton_44a_pressed();

    //保存采集的温度数据
    void on_pushButton_21b_pressed();

    //manual slots function
    void showListMessage();
    void showTemperInLcd(quint16);
    void showSubUI(QString);

private:
    Ui::MainWindow *ui;
    SpNode spnode;
    QTimer *pollTimer;
    CurveWindow *independent_ui;
    QPointer<CurveWindow> m_ptr = nullptr;

    //记录数据
    int time_point=0;
    //std::queue<int> *temper_queue_ptr;
    //当前温度
    float *c_temper_ptr = new float(0.0);
};

#endif // MAINWINDOW_H
