#ifndef ALCONDUCTOR_H
#define ALCONDUCTOR_H
#include "customsocketserver.h"
#include "conductor/conductor.h"

class AlCallback;

class AlManager
{

public:
    AlManager();
    /* use virtual otherwise linker will try to perform static linkage */
    virtual void init(AlCallback* alCallback);
    virtual void run();

    virtual bool hasConnections() {
        return (m_conductor->m_peerConnection.get());
    }

//    virtual rtc::scoped_refptr<Conductor> getConductor() {
//        std::cout << "aaaa" << std::endl;
//        return m_conductor;
//    }

private:
    rtc::AutoThread auto_thread;
    rtc::Thread* thread;
    CustomSocketServer* m_socketServer;
    rtc::scoped_refptr<Conductor> m_conductor;
public:
    PeerConnectionClient m_client;
};

#endif // ALCONDUCTOR_H
