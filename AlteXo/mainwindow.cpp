#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    connect(this, SIGNAL(addMaxDepthSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(changeMaxDepth(int)));
//    connect(this, SIGNAL(substractMaxDepthSignal(int)), ui->alGLWidget->getALKinectInterface(), SLOT(changeMaxDepth(int)));

    this->videoSurface = new ALVideoSurface(this);
    QGridLayout *videoLayout = new QGridLayout();
    videoLayout->addWidget(videoSurface);
    this->ui->videoFrame->setLayout(videoLayout);

    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(requestNewFrameSlot()));
    timer->start(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestNewFrameSlot() {
//    qDebug() << "request new frame";
    Q_EMIT this->requestNewFrameSignal();
}

ALVideoSurface* MainWindow::getVideoSurfaceRef() {
    return this->videoSurface;
}

void MainWindow::on_startRecorder_clicked()
{
    if (this->ui->startRecorder->text() == "Record") {
        qDebug() << this->ui->startRecorder->text();
        this->timer->stop();
        Q_EMIT this->startRecorderSignal();

        this->ui->startRecorder->setText("Stop");
        this->ui->startRecorder->setStyleSheet("QPushButton {color: red;}");
    } else {
        timer->start(40);
        Q_EMIT this->stopRecorderSignal();
        this->ui->startRecorder->setText("Record");
        this->ui->startRecorder->setStyleSheet("QPushButton {color: black;}");
    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog sDialog;
    this->connect(&sDialog, SIGNAL(settingsChangedSignal()), this, SLOT(settingsChangedSlot()));
    sDialog.setModal(true);
    int ret = sDialog.exec();
    qDebug() << ret;
    switch (ret) {
        case QDialog::Accepted:
        {
            qDebug() << "ok clicked";
            break;
        }
        case QDialog::Rejected:
        {
            qDebug() << "cancel clicked";
            break;
        }
        default:
            // should never be reached
            qDebug() << "default behaviour";
            qDebug() << QDialogButtonBox::Ok;
            qDebug() << QDialogButtonBox::Cancel;
            break;
    }
}

void MainWindow::settingsChangedSlot()
{
    Q_EMIT this->settingsChangedSignal();
}
