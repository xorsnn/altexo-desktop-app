#ifndef ALCONDUCTOR_H
#define ALCONDUCTOR_H
#include "customsocketserver.h"
#include "conductor/conductor.h"
#include "conductor/aldatamanager.h"

class AlCallback;

class AlManager
{

public:
    AlManager();
    /* use virtual otherwise linker will try to perform static linkage */
    virtual void init(AlCallback* alCallback);
    virtual void run();

    virtual bool hasConnections();
    virtual void InitializePeerConnection();
    virtual void DeletePeerConnection();

//    virtual rtc::scoped_refptr<Conductor> getConductor() {
//        std::cout << "aaaa" << std::endl;
//        return m_conductor;
//    }
    virtual void OnMessageFromPeer(std::string peer_id, const std::string& message);

private:
    rtc::AutoThread auto_thread;
    rtc::Thread* thread;
    CustomSocketServer* m_socketServer;
    rtc::scoped_refptr<Conductor> m_conductor;
    AlDataManager m_dataManager;
public:
    PeerConnectionClient m_client;
};

#endif // ALCONDUCTOR_H
