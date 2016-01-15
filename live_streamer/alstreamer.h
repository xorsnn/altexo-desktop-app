#ifndef ALSTREAMER_H
#define ALSTREAMER_H

#include <QObject>
#include "customsocketserver.h"

class AlStreamer : public QObject
{
    Q_OBJECT
public:
    explicit AlStreamer(QObject *parent = 0);


public Q_SLOTS:
    void initSlot(QApplication* atApp);
    void runSlot();

    rtc::scoped_refptr<AlConductor> getConductorSlot();

private:
    rtc::AutoThread auto_thread;
    rtc::Thread* thread;
    CustomSocketServer* socket_server;

};

#endif // ALSTREAMER_H
