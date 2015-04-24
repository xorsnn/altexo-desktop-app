#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(addMaxDepthSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(changeMaxDepth(int)));
    connect(this, SIGNAL(substractMaxDepthSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(changeMaxDepth(int)));
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

void MainWindow::on_substractMaxDepth_clicked()
{
    emit this->substractMaxDepthSignal(-100);
}

void MainWindow::on_addMaxDepth_clicked()
{
    emit this->addMaxDepthSignal(100);
}
