#include <QApplication>
#include <QDebug>
#include <QTextEdit>

//#include <gtk/gtk.h>

//#include "webrtc/examples/peerconnection/client/conductor.h"
//#include "webrtc/examples/peerconnection/client/flagdefs.h"
//#include "webrtc/examples/peerconnection/client/linux/main_wnd.h"
//#include "webrtc/examples/peerconnection/client/peer_connection_client.h"
#include "webrtc/base/physicalsocketserver.h"
#include "webrtc/base/flags.h"

#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"

#include "alinterface/almainwindow.h"
#include "alconductor/alconductor.h"
#include "alinterface/widgetstreamer.h"

class CustomSocketServer : public rtc::PhysicalSocketServer {
public:
    CustomSocketServer(rtc::Thread* thread, /*GtkMainWnd* wnd, */QApplication* qtApp)
        : thread_(thread), /*wnd_(wnd),*/ qtApp_(qtApp), conductor_(NULL)/*, client_(NULL)*/ {}
    virtual ~CustomSocketServer() {}

//    void set_client(PeerConnectionClient* client) { client_ = client; }
    void set_conductor(AlConductor* conductor) { conductor_ = conductor; }

    // Override so that we can also pump the GTK message loop.
    virtual bool Wait(int cms, bool process_io) {
        // Pump GTK events.
        // TODO(henrike): We really should move either the socket server or UI to a
        // different thread.  Alternatively we could look at merging the two loops
        // by implementing a dispatcher for the socket server and/or use
        // g_main_context_set_poll_func.
//        qDebug() << "test 2";
//        while (gtk_events_pending()) {
//            gtk_main_iteration();
////            qDebug() << "test 3";
//        }
        qtApp_->processEvents();
//        qDebug() << "test 4";
        if (/*!wnd_->IsWindow() &&*/ !conductor_->connection_active()/* &&
                client_ != NULL && !client_->is_connected()*/) {
            thread_->Quit();
        }
//        return rtc::PhysicalSocketServer::Wait(0/*cms == -1 ? 1 : cms*/,
//                                               process_io);
        return rtc::PhysicalSocketServer::Wait(cms == -1 ? 1 : cms,
                                               process_io);
    }

protected:
    rtc::Thread* thread_;
//    GtkMainWnd* wnd_;
    QApplication* qtApp_;
    AlConductor* conductor_;
//    PeerConnectionClient* client_;
};

int main(int argc, char *argv[])
{
    QApplication qtApp(argc, argv);
    QTextEdit w1;
    AlMainWindow w;

    w.show();
    w1.show();

//    gtk_init(&argc, &argv);
//    g_type_init();
//    // g_thread_init API is deprecated since glib 2.31.0, see release note:
//    // http://mail.gnome.org/archives/gnome-announce-list/2011-October/msg00041.html
//#if !GLIB_CHECK_VERSION(2, 31, 0)
//    g_thread_init(NULL);
//#endif

//    rtc::FlagList::SetFlagsFromCommandLine(&argc, argv, true);
//    if (FLAG_help) {
//        rtc::FlagList::Print(NULL, false);
//        return 0;
//    }

    // Abort if the user specifies a port that is outside the allowed
    // range [1, 65535].
//    if ((FLAG_port < 1) || (FLAG_port > 65535)) {
//        printf("Error: %i is not a valid port.\n", FLAG_port);
//        return -1;
//    }

//    GtkMainWnd wnd(FLAG_server, FLAG_port, FLAG_autoconnect, FLAG_autocall);
//    wnd.Create();

    rtc::AutoThread auto_thread;
    rtc::Thread* thread = rtc::Thread::Current();
    CustomSocketServer socket_server(thread, &qtApp);
    thread->set_socketserver(&socket_server);

    rtc::InitializeSSL();
    // Must be constructed after we set the socketserver.
//    PeerConnectionClient client;
//    rtc::scoped_refptr<Conductor> conductor(
//                new rtc::RefCountedObject<Conductor>(&client, &wnd));
    rtc::scoped_refptr<AlConductor> conductor(
                new rtc::RefCountedObject<AlConductor>());
//    socket_server.set_client(&client);
    socket_server.set_conductor(conductor);

    w.setConductor(conductor);
    QObject::connect(&w, SIGNAL(signalStartButton_clicked()), conductor, SLOT(StartAll()));
    QObject::connect(&w, SIGNAL(signalProcessAnswerButton_clicked(QString)), conductor, SLOT(slotProcessAnswer(QString)));
    QObject::connect(&w, SIGNAL(signalProcessRemoteICEButton_clicked(QString)), conductor, SLOT(slotProcessRemoteICE(QString)));
    QObject::connect(conductor, SIGNAL(signalSDPText(QString)), &w, SLOT(slotSDPText(QString)));
    QObject::connect(conductor, SIGNAL(signalOnLocalIceCandidate(QString)), &w, SLOT(slotOnLocalIceCandidate(QString)));

    WidgetStreamer conn(&w1, conductor);

    qDebug() << "test 1";
    thread->Run();

    // gtk_main();
//    wnd.Destroy();

    thread->set_socketserver(NULL);
    // TODO(henrike): Run the Gtk main loop to tear down the connection.
    /*
      while (gtk_events_pending()) {
        gtk_main_iteration();
      }
      */
    rtc::CleanupSSL();
    return 0;

    //    return a.exec();
}
