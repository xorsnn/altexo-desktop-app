#include "alsdkinterface.hpp"
#include "allogger.hpp"
#include "customsocketserver.h"
#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"

AlSdkInterface::AlSdkInterface() {}

AlSdkInterface::~AlSdkInterface() {}

void AlSdkInterface::run() {
  alLogger() << "some!";
  rtc::AutoThread auto_thread;
  rtc::Thread *thread = rtc::Thread::Current();
  CustomSocketServer socket_server(thread);
  thread->set_socketserver(&socket_server);

  rtc::InitializeSSL();
  // Must be constructed after we set the socketserver.
  // PeerConnectionClient client;
  rtc::scoped_refptr<AlConductor> conductor(
      new rtc::RefCountedObject<AlConductor>());
  // // socket_server.set_client(&client);
  socket_server.set_conductor(conductor);
  thread->Run();

  thread->set_socketserver(NULL);
  rtc::CleanupSSL();
  alLogger() << "fin!";
}
