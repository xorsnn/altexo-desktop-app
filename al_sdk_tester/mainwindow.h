#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "alconnclient.h"

class MainWndCallback {
public:
    virtual void StartLogin(const std::string& server, int port) = 0;
    virtual void DisconnectFromServer() = 0;
    virtual void ConnectToPeer(QString peer_id) = 0;
    virtual void DisconnectFromCurrentPeer() = 0;
//    virtual void UIThreadCallback(int msg_id, void* data) = 0;
    virtual void Close() = 0;
protected:
    virtual ~MainWndCallback() {}
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void RegisterObserver(MainWndCallback* callback);
    void setWsClient(AlConnClient* cl);

public Q_SLOTS:
    void OnPeerConnectedSlot(QString id, QString name);
    void onNewFrameSlot(QImage* img);
    void onDevicesListChangedSlot(std::vector<std::string> device_names);
    void timeoutSlot();
private Q_SLOTS:
    void on_peerListView_doubleClicked(const QModelIndex &index);

    void on_videoDevicesView_doubleClicked(const QModelIndex &index);

Q_SIGNALS:
    void ConnectToPeerSignal(QString peer_id);
    void selectVideoDeviceSignal(QString deviceName);

private:
    Ui::MainWindow *ui;
    MainWndCallback* m_callback;
    AlConnClient* m_cl;
    bool m_debug;
    QStringListModel* m_model;
    QStringListModel* m_devicesListModel;
};

#endif // MAINWINDOW_H
