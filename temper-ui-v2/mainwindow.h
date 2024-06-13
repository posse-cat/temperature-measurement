#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QLabel>
#include <QMainWindow>

//&&&&&&&&&&&&&&&  前置声明  &&&&&&&&&&&&&&&
class TcpNode;
class CurvePattern;
class CloudMapWidget;
class TcpConfWindow;
class FiberConfWindow;
namespace Ui {
class MainWindow;
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int compute_bool2int(bool);
    void setLED(QLabel *label, int color, int size);
    void setConnectStateDisp(QLabel*label,int mode);
    void createCurveObject();
    void setTotalChart();
    void initCloudMap();
    //virtual void paintEvent(QPaintEvent*) override;

public Q_SLOTS:
    void on_action_connect_triggered();
    void on_action_disconnect_triggered();
    void on_action_calibration_triggered();
    void on_pushButton_cr_list_pressed();

    void updateTcpConnectionState(bool);
    void updateCurveDisplay();
    void drawCurveOnBox();
    void showMessage(QString);

private:
    Ui::MainWindow *ui;

    TcpNode * tcp_node;

    TcpConfWindow *tcp_window;
    QPointer<TcpConfWindow> tcp_ui_ptr;

    FiberConfWindow * fiber_window;
    QPointer<FiberConfWindow> fiber_ui_ptr;

    // chart in total widget
    QSharedPointer<CurvePattern>curve_ch1_t;
    QSharedPointer<CurvePattern>curve_ch2_t;
    QSharedPointer<CurvePattern>curve_ch3_t;
    QSharedPointer<CurvePattern>curve_ch4_t;
    //chart in local widget
    QSharedPointer<CurvePattern>curve_ch1_l;
    QSharedPointer<CurvePattern>curve_ch2_l;
    QSharedPointer<CurvePattern>curve_ch3_l;
    QSharedPointer<CurvePattern>curve_ch4_l;

    //cloud map
    QSharedPointer<CloudMapWidget> surface_map1;
    QSharedPointer<CloudMapWidget> surface_map2;
    QSharedPointer<CloudMapWidget> surface_map3;
    QSharedPointer<CloudMapWidget> surface_map4;

    //指示灯圆的半径
    int radius=32;
    //消息显示
    QStringListModel message_list;
};

#endif // MAINWINDOW_H
