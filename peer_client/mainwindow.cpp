#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    debug_(true)
{
    ui->setupUi(this);

    this->m_model = new QStringListModel();
    this->ui->peerListView->setModel(this->m_model);
    this->ui->peerListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// **
// * MainWnd implementation
// *

void MainWindow::RegisterObserver(MainWndCallback* callback) {
    if (this->debug_) {
        qDebug() << "MainWindow::RegisterObserver";
    }

    this->callback_ = callback;
}

bool MainWindow::IsWindow() {
    return true; //???
}

void MainWindow::MessageBox(const char* caption, const char* text,
                            bool is_error) {
    if (this->debug_) {
        qDebug() << "MainWindow::MessageBox";
    }
    qDebug() << QString::fromUtf8(text);
}

MainWindow::UI MainWindow::current_ui() {
//    if (vbox_)
//        return CONNECT_TO_SERVER;

//    if (peer_list_)
//        return LIST_PEERS;

    return STREAMING;
}

void MainWindow::SwitchToConnectUI() {
    // seems like just a layout switch
    if (this->debug_) {
        qDebug() << "MainWindow::SwitchToConnectUI";
    }
}

void MainWindow::SwitchToPeerList(const Peers& peers) {
    // seems like just a layout switch
    if (this->debug_) {
        qDebug() << "MainWindow::SwitchToPeerList";
    }
}

void MainWindow::SwitchToStreamingUI() {
    // seems like just a layout switch
    if (this->debug_) {
        qDebug() << "MainWindow::SwitchToStreamingUI";
    }
}

void MainWindow::StartLocalRenderer(webrtc::VideoTrackInterface* local_video) {
    if (this->debug_) {
        qDebug() << "MainWindow::StartLocalRenderer";
    }
    this->local_renderer_.reset(new VideoRenderer(0, local_video));
}

void MainWindow::StopLocalRenderer() {
    if (this->debug_) {
        qDebug() << "MainWindow::StopLocalRenderer(";
    }
    this->local_renderer_.reset();
}

// TODO
void MainWindow::updateRenderImage(const int target,QImage *newImage) {
    qDebug() << "MainWindow::updateRenderImage";
    QLabel *drawed = NULL;
    drawed = this->ui->_remoteLabel;
    if (newImage->isNull()) {
        qDebug() << "newImage->isNull";
    }
    if(drawed != NULL && !newImage->isNull() && newImage->width() > 0 && newImage->height() > 0)
    {
        qDebug() << "MainWindow::updateRenderImage --- 1";
        drawed->setFixedSize(newImage->width(),newImage->height());
        drawed->setPixmap(QPixmap::fromImage(*newImage));
        drawed->show();
    }
    remote_renderer_->turnUpdated();
}

void MainWindow::StartRemoteRenderer(
        webrtc::VideoTrackInterface* remote_video) {
    if (this->debug_) {
        qDebug() << "MainWindow::StartRemoteRenderer";
    }
    this->remote_renderer_.reset(new VideoRenderer(1, remote_video));
    QObject::connect(this->remote_renderer_.get(),SIGNAL(imageToUpdate(const int,QImage *)),
                    this,SLOT(updateRenderImage(const int,QImage *)));
}

void MainWindow::StopRemoteRenderer() {
    if (this->debug_) {
        qDebug() << "MainWindow::StopRemoteRenderer";
    }
    this->remote_renderer_.reset();
}

void MainWindow::QueueUIThreadCallback(int msg_id, void* data) {
    if (this->debug_) {
        qDebug() << "MainWindow::QueueUIThreadCallback";
//        qDebug() << msg_id;
    }
    // todo some kind of a lame solution
    this->callback_->UIThreadCallback(msg_id, data);
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
    this->m_cl = cl;
}

void MainWindow::on_startButton_clicked()
{
    //    gtk_widget_set_sensitive(widget, false);
    this->server_ = "127.0.0.1";
    this->port_ = "8888";
    int port = this->port_.length() ? atoi(this->port_.c_str()) : 0;
    callback_->StartLogin(this->server_, port);
}



void MainWindow::on_peerListView_doubleClicked(const QModelIndex &index)
{
    qDebug() << this->m_model->data(index, Qt::EditRole).toString();
    Q_EMIT ConnectToPeerSignal(this->m_model->data(index, Qt::EditRole).toString());
}
