#ifndef CUSTOMSOCKETSERVER_H
#define CUSTOMSOCKETSERVER_H
#include "webrtc/base/physicalsocketserver.h"
#include "conductor/conductor.h"
#include "alcallback.h"
#include <iostream>

//class AlCallback;

class CustomSocketServer : public rtc::PhysicalSocketServer {
public:

    CustomSocketServer(rtc::Thread* thread)
        : m_thread(thread), m_conductor(NULL), m_alCallback(NULL) {}
    virtual ~CustomSocketServer() {}

    void set_conductor(rtc::scoped_refptr<Conductor> conductor) { m_conductor = conductor; }
    void setAlCallback(AlCallback* alCallback) { m_alCallback = alCallback; }

    //TODO - introduce callback!!!
    // Override so that we can also pump the GTK message loop.
    virtual bool Wait(int cms, bool process_io) {
//        std::cout << "ping!" << std::endl;

        // Pump GTK events.
        // TODO(henrike): We really should move either the socket server or UI to a
        // different thread.  Alternatively we could look at merging the two loops
        // by implementing a dispatcher for the socket server and/or use
        // g_main_context_set_poll_func.
        //TODO make a callback for this
//        qtApp_->processEvents();
        // TODO false is temporary solution
        if (m_alCallback) {
            m_alCallback->processUiEventsCb();
        }
        if (false && !m_conductor->connection_active()) {
            m_thread->Quit();
        }
//        return rtc::PhysicalSocketServer::Wait(0/*cms == -1 ? 1 : cms*/,
//                                               process_io);
        return rtc::PhysicalSocketServer::Wait(cms == -1 ? 1 : cms,
                                               process_io);
    }

    Conductor* getConductor() {
        return m_conductor;
    }

protected:
    rtc::Thread* m_thread;
    rtc::scoped_refptr<Conductor> m_conductor;
    AlCallback* m_alCallback;
};

#endif // CUSTOMSOCKETSERVER_H
