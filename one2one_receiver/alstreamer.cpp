#include "alstreamer.h"
#include <QDebug>
#include <QTimer>
#include "webrtc/base/ssladapter.h"

AlStreamer::AlStreamer(QObject *parent) :
    QObject(parent)
{
}

void AlStreamer::initSlot(QGuiApplication* qtApp) {
//    kind of thread isssue using QT
    qDebug() << "init slot";
    this->thread = rtc::Thread::Current();
    this->socket_server = new CustomSocketServer(thread, qtApp);
    thread->set_socketserver(this->socket_server);
    rtc::InitializeSSL();
    // Must be constructed after we set the socketserver.
    rtc::scoped_refptr<AlConductor> conductor(
                new rtc::RefCountedObject<AlConductor>());
    this->socket_server->set_conductor(conductor);
    qDebug() << "init slot fin";
//    conductor.alInit();
//    QMetaObject::invokeMethod(this->getConductorSlot(), "alInit", Qt::QueuedConnection);
//    QTimer::singleShot(3000, this->getConductorSlot(), SLOT(alInit()));
}

void AlStreamer::runSlot() {
    qDebug() << "run slot";
    this->thread->Run();
    this->thread->set_socketserver(NULL);
    rtc::CleanupSSL();
}

rtc::scoped_refptr<AlConductor> AlStreamer::getConductorSlot() {
    return this->socket_server->getConductor();
}
