#include "alstreamer.h"
#include <QDebug>
#include "webrtc/base/ssladapter.h"

AlStreamer::AlStreamer(QObject *parent) :
    QObject(parent)
{
}

void AlStreamer::initSlot(QGuiApplication* qtApp) {
//    kind of thread isssue using QT
    this->thread = rtc::Thread::Current();
    this->socket_server = new CustomSocketServer(thread, qtApp);
    thread->set_socketserver(this->socket_server);
    rtc::InitializeSSL();
    // Must be constructed after we set the socketserver.
    rtc::scoped_refptr<Conductor> conductor(
                new rtc::RefCountedObject<Conductor>());
    this->socket_server->set_conductor(conductor);
}

void AlStreamer::runSlot() {
    this->thread->Run();
    this->thread->set_socketserver(NULL);
    rtc::CleanupSSL();
}

rtc::scoped_refptr<Conductor> AlStreamer::getConductorSlot() {
    return this->socket_server->getConductor();
}
