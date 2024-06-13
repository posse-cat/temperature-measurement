#ifndef FIBERCONFWINDOW_H
#define FIBERCONFWINDOW_H

#include <QMainWindow>

namespace Ui {
class FiberConfWindow;
}

class FiberConfWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FiberConfWindow(QWidget *parent = nullptr);
    ~FiberConfWindow();

private:
    Ui::FiberConfWindow *ui;
};

#endif // FIBERCONFWINDOW_H
