#ifndef CUSTOMSOCKETSERVER_H
#define CUSTOMSOCKETSERVER_H
#include "webrtc/base/physicalsocketserver.h"
#include "alconductor/alconductor.h"
#include <QApplication>

class CustomSocketServer : public rtc::PhysicalSocketServer {
public:
    CustomSocketServer(rtc::Thread* thread, QApplication* qtApp)
        : thread_(thread), qtApp_(qtApp), conductor_(NULL) {}
    virtual ~CustomSocketServer() {}

    void set_conductor(rtc::scoped_refptr<AlConductor> conductor) { conductor_ = conductor; }

    // Override so that we can also pump the GTK message loop.
    virtual bool Wait(int cms, bool process_io) {
        // Pump GTK events.
        // TODO(henrike): We really should move either the socket server or UI to a
        // different thread.  Alternatively we could look at merging the two loops
        // by implementing a dispatcher for the socket server and/or use
        // g_main_context_set_poll_func.
        qtApp_->processEvents();
        if (!conductor_->connection_active()) {
            thread_->Quit();
        }
//        return rtc::PhysicalSocketServer::Wait(0/*cms == -1 ? 1 : cms*/,
//                                               process_io);
        return rtc::PhysicalSocketServer::Wait(cms == -1 ? 1 : cms,
                                               process_io);
    }

    AlConductor* getConductor() {
        return this->conductor_;
    }

protected:
    rtc::Thread* thread_;
    QApplication* qtApp_;
    rtc::scoped_refptr<AlConductor> conductor_;
};

#endif // CUSTOMSOCKETSERVER_H
