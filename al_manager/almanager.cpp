#include "almanager.h"
#include <iostream>
#include "webrtc/base/ssladapter.h"

using namespace std;

extern "C" AlManager* create_object()
{
    return new AlManager;
}

extern "C" void destroy_object( AlManager* object )
{
    delete object;
}

AlManager::AlManager():
    m_conductor(NULL)
{
}


void AlManager::init(AlCallback* alCallback)
{
    this->thread = rtc::Thread::Current();
    m_socketServer = new CustomSocketServer(thread);
    thread->set_socketserver(m_socketServer);
    rtc::InitializeSSL();
    // Must be constructed after we set the socketserver.
    rtc::scoped_refptr<Conductor> conductor(
                new rtc::RefCountedObject<Conductor>(&m_client, alCallback));
    m_socketServer->set_conductor(conductor);
    m_socketServer->setAlCallback(alCallback);
    m_conductor = conductor;
}

void AlManager::run()
{
    this->thread->Run();
    this->thread->set_socketserver(NULL);
    rtc::CleanupSSL();
}
