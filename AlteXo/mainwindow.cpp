#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPluginLoader>
#include <QDir>
#include <QJsonDocument>
#include "interfaces/AlStreamerInterface.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
    QString room = settings.value("altexo/alRoom", "altexo-chat").toString();
    this->ui->roomEdit->setText(room);

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

void MainWindow::on_streamButton_clicked()
{
    qDebug() << "stream";
    Q_EMIT this->signalStartButton_clicked();
}

void MainWindow::on_StartButton_clicked()
{
    Q_EMIT this->signalStartButton_clicked();
}

void MainWindow::on_pProcessAnswerButton_clicked()
{
    Q_EMIT this->signalProcessAnswerButton_clicked(this->ui->pAnswerText->toPlainText());

}

void MainWindow::on_pProcessRemoteICEButton_clicked()
{
    Q_EMIT this->signalProcessRemoteICEButton_clicked(this->ui->pRemoteICEText->toPlainText());
}

void MainWindow::slotSDPText(const QString &sdp) {
    qDebug() << "AlMainWindow::slotSDPText";
    this->ui->pOfferText->setPlainText(sdp);
}

void MainWindow::slotOnLocalIceCandidate(const QString &iceCandidate)
{
    QString str = this->ui->pOwnICEText->toPlainText();
    str += iceCandidate + "\n";
    this->ui->pOwnICEText->setPlainText(str);
}

void MainWindow::onJsonMsgSlot(QString msg) {
    qDebug() << "MainWindow::onJsonMsgSlot";
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject jsonObj = doc.object();
    if (jsonObj["type"].toString() == "SDP") {
        this->ui->pAnswerText->setPlainText(jsonObj["body"].toString());
        QTimer::singleShot(1000, this, SLOT(sendIceCandidatesSlot()));
    } else if (jsonObj["type"].toString() == "ICE") {
        this->ui->pRemoteICEText->setPlainText(jsonObj["body"].toString());
        Q_EMIT this->readyToStreamSignal();
    }
}

void MainWindow::sendIceCandidatesSlot() {
    qDebug() << "MainWindow::sendIceCandidatesSlot";
    Q_EMIT this->sendIceCandidatesSignal(this->ui->pOwnICEText->toPlainText());
}

void MainWindow::on_roomEdit_textChanged(const QString &arg1)
{
    QSettings settings;
    settings.setValue("altexo/alRoom", arg1);
}
