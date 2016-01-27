#ifndef ALMAINWINDOW_H
#define ALMAINWINDOW_H

#include <QMainWindow>
#include "alconductor/alconductor.h"

namespace Ui {
class AlMainWindow;
}

class AlMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AlMainWindow(QWidget *parent = 0);
    ~AlMainWindow();

    void setConductor(rtc::scoped_refptr<AlConductor> conductor);

public Q_SLOTS:
    void slotSDPText(const QString &sdp);
    void slotOnLocalIceCandidate(const QString &iceCandidate);

private Q_SLOTS:
    void on_StartButton_clicked();

    void on_pProcessAnswerButton_clicked();

    void on_pProcessRemoteICEButton_clicked();

private:
    Ui::AlMainWindow *ui;

//    rtc::scoped_refptr<AlConductor> conductor_;

Q_SIGNALS:
    void signalStartButton_clicked();
    void signalProcessAnswerButton_clicked(QString sdpInfo);
    void signalProcessRemoteICEButton_clicked(QString iceStr);
};

#endif // ALMAINWINDOW_H
