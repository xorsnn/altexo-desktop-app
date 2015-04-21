#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray MainWindow::getFrame() {
//    qDebug() << "test slot";
    return ui->alGLWidget->getFrame();
}

void MainWindow::gstBufferFill(GstBuffer *buffer, guint size) {
    ui->alGLWidget->gstBufferFill(buffer, size);
//    int i = 1;
//    i = 2;
//    i+=1;
//    i+=1;
}

void MainWindow::on_startRecorder_clicked()
{
    emit this->startRecorder();
}

void MainWindow::on_stopRecorder_clicked()
{
    qDebug() << "stop to fire";
    emit this->stopRecorder();
}
