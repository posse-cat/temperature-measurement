#include "fiberconfwindow.h"
#include "ui_fiberconfwindow.h"

FiberConfWindow::FiberConfWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiberConfWindow)
{
    ui->setupUi(this);
}

FiberConfWindow::~FiberConfWindow()
{
    delete ui;
}
