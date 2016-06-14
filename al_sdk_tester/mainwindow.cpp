#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_debug(true)
{
    ui->setupUi(this);

    m_model = new QStringListModel();
    ui->peerListView->setModel(this->m_model);
    ui->peerListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_devicesListModel = new QStringListModel();
    ui->videoDevicesView->setModel(m_devicesListModel);
    ui->videoDevicesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
//    timer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_model;
    m_model = NULL;
    delete m_devicesListModel;
    m_devicesListModel = NULL;
}


void MainWindow::timeoutSlot() {
    qDebug() << "MainWindow::timeoutSlot";
}

void MainWindow::RegisterObserver(MainWndCallback* callback) {
    if (m_debug) {
        qDebug() << "MainWindow::RegisterObserver";
    }

    m_callback = callback;
}

// **
// * WS CLIENT MANAGEMENT
// *
void MainWindow::OnPeerConnectedSlot(QString id, QString name) {
    qDebug() << "MainWindow::OnPeerConnectedSlot";
    QStringList list;
    QMapIterator<QString, QString> i(this->m_cl->getPeers());

    while (i.hasNext()) {
        i.next();
        list << i.key();
    }

    this->m_model->setStringList(list);
}

void MainWindow::setWsClient(AlConnClient* cl) {
    m_cl = cl;
}

void MainWindow::on_peerListView_doubleClicked(const QModelIndex &index)
{
    qDebug() << this->m_model->data(index, Qt::EditRole).toString();
    Q_EMIT ConnectToPeerSignal(this->m_model->data(index, Qt::EditRole).toString());
}

void MainWindow::onNewFrameSlot(QImage* img) {
    QLabel *drawed = NULL;
    drawed = this->ui->videoView;
    if (img->isNull()) {
        qDebug() << "newImage->isNull";
    }
    if(drawed != NULL && !img->isNull() && img->width() > 0 && img->height() > 0)
    {
//        qDebug() << "MainWindow::updateRenderImage --- 1";
        drawed->setFixedSize(img->width(),img->height());
        drawed->setPixmap(QPixmap::fromImage(*img));
        drawed->show();
    }
//    remote_renderer_->turnUpdated();
}

void MainWindow::onDevicesListChangedSlot(std::vector<std::string> device_names) {
    QStringList list;
    for (const auto& name : device_names) {
        list << QString::fromStdString(name);
    }
    m_devicesListModel->setStringList(list);
}

void MainWindow::on_videoDevicesView_doubleClicked(const QModelIndex &index)
{
//    qDebug() << this->m_model->data(index, Qt::EditRole).toString();
    Q_EMIT selectVideoDeviceSignal(this->m_devicesListModel->data(index, Qt::EditRole).toString());
}
