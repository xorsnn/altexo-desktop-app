#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    connect(this, SIGNAL(addMaxDepthSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(changeMaxDepth(int)));
//    connect(this, SIGNAL(substractMaxDepthSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(changeMaxDepth(int)));

//    connect(this, SIGNAL(moveHorizontalSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(moveHorizontalSlot(int)));
//    connect(this, SIGNAL(moveVerticalSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(moveVerticalSlot(int)));
    this->videoSurface = new ALVideoSurface(this);
    QVBoxLayout *videoLayout = new QVBoxLayout(this);
//    QPushButton *btnOk = new QPushButton("OK");
    videoLayout->addWidget(videoSurface);
    this->ui->videoFrame->setLayout(videoLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ALVideoSurface* MainWindow::getVideoSurfaceRef() {
    return this->videoSurface;
}

//QByteArray MainWindow::getFrame() {
//    return ui->alGLWidget->getFrame();
//}

//void MainWindow::bufferFill(quint8 *buffer, uint size) {
//    ui->alGLWidget->bufferFill(buffer, size);
//}

//void MainWindow::gstBufferFill(GstBuffer *buffer, guint size) {
//    ui->alGLWidget->gstBufferFill(buffer, size);
//}

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

//ALGLWidget* MainWindow::getALGLWidget() {
//    return this->ui->alGLWidget;
//}
