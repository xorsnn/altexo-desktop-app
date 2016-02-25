#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
#include <QDialogButtonBox>
#include "alvideosurface.h"
#include "settingsdialog.h"

//#include "alkinectinterface.h"
//#include "alglwidget.h"
//#include <gst/gst.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    ALVideoSurface* getVideoSurfaceRef();
    void requestNewFrameSlot();
    void settingsChangedSlot();

//streamer
    void slotSDPText(const QString &sdp);
    void slotOnLocalIceCandidate(const QString &iceCandidate);

    void onJsonMsgSlot(QString msg);
    void sendIceCandidatesSlot();

private Q_SLOTS:
    void on_startRecorder_clicked();
    void on_actionSettings_triggered();

    void on_streamButton_clicked();

    void on_StartButton_clicked();

    void on_pProcessAnswerButton_clicked();

    void on_pProcessRemoteICEButton_clicked();

    void on_roomEdit_textChanged(const QString &arg1);

    void readyToStreamSlot();

private:
    Ui::MainWindow *ui;
    ALVideoSurface * videoSurface;
    QTimer *timer;
Q_SIGNALS:
    void settingsChangedSignal();

    void startRecorderSignal();
    void stopRecorderSignal();

    void requestNewFrameSignal();

//======================
//==============streamer
//======================
    void signalStartButton_clicked();
    void signalProcessAnswerButton_clicked(QString msg);
    void signalProcessRemoteICEButton_clicked(QString msg);

    void sendIceCandidatesSignal(QString msg);

    void sendTextMessageSignal(QString msg);

    void readyToStreamSignal();
};

#endif // MAINWINDOW_H
