
// #include <boost/log/core.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/sinks/text_file_backend.hpp>
// #include <boost/log/sources/global_logger_storage.hpp>
// #include <boost/log/sources/record_ostream.hpp>
// #include <boost/log/sources/severity_logger.hpp>
// #include <boost/log/trivial.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>
// #include <boost/log/utility/setup/file.hpp>
// #include <boost/smart_ptr/make_shared_object.hpp>
// #include <boost/smart_ptr/shared_ptr.hpp>
// #include <fstream>
// #include <iostream>
// #include <stdio.h>

// #include "conductor/conductor.h"
// #include "conductor/flagdefs.h"
// #include "conductor/main_wnd.h"
// #include "conductor/peer_connection_client.h"

#include "allogger.hpp"
#include "customsocketserver.h"
#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"

// class CustomSocketServer : public rtc::PhysicalSocketServer {
// public:
//   CustomSocketServer(rtc::Thread *thread, GtkMainWnd *wnd)
//       : thread_(thread), wnd_(wnd), conductor_(NULL), client_(NULL) {}
//   virtual ~CustomSocketServer() {}
//
//   void set_client(PeerConnectionClient *client) { client_ = client; }
//   void set_conductor(Conductor *conductor) { conductor_ = conductor; }
//
//   // Override so that we can also pump the GTK message loop.
//   virtual bool Wait(int cms, bool process_io) {
//     // Pump GTK events.
//     // TODO(henrike): We really should move either the socket server or UI to
//     a
//     // different thread.  Alternatively we could look at merging the two
//     loops
//     // by implementing a dispatcher for the socket server and/or use
//     // g_main_context_set_poll_func.
//     while (gtk_events_pending())
//       gtk_main_iteration();
//
//     if (!wnd_->IsWindow() && !conductor_->connection_active() &&
//         client_ != NULL && !client_->is_connected()) {
//       thread_->Quit();
//     }
//     return rtc::PhysicalSocketServer::Wait(0 /*cms == -1 ? 1 : cms*/,
//                                            process_io);
//   }
//
// protected:
//   rtc::Thread *thread_;
//   GtkMainWnd *wnd_;
//   Conductor *conductor_;
//   PeerConnectionClient *client_;
// };

int main(int, char **) {
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
  return 0;
}
