#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QObject>

#include "conductor.h"
#include "flagdefs.h"
#include "peer_connection_client.h"

#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"

#include <QtQuick/QQuickView>
#include "quick_renderer/threadrenderer.h"

#include "alconnclient.h"
#include <dlfcn.h>
#include "almanager.h"
#include "peerclientcallback.h"


//class CustomSocketServer : public rtc::PhysicalSocketServer {
//public:
//    CustomSocketServer(rtc::Thread* thread, QGuiApplication* wnd)
//        : thread_(thread), wnd_(wnd), conductor_(NULL), client_(NULL) {}
//    virtual ~CustomSocketServer() {}

//    void set_client(PeerConnectionClient* client) { client_ = client; }
//    void set_conductor(Conductor* conductor) { conductor_ = conductor; }

//    // Override so that we can also pump the GTK message loop.
//    virtual bool Wait(int cms, bool process_io) {
//        // Pump GTK events.
//        // TODO(henrike): We really should move either the socket server or UI to a
//        // different thread.  Alternatively we could look at merging the two loops
//        // by implementing a dispatcher for the socket server and/or use
//        // g_main_context_set_poll_func.

//        wnd_->processEvents();

////        qDebug() << "ping!";

//        if (false && !conductor_->connection_active() &&
//                client_ != NULL && !client_->is_connected()) {
//            qDebug() << "pong!";
//            thread_->Quit();
//        }
////        return rtc::PhysicalSocketServer::Wait(0/*cms == -1 ? 1 : cms*/,
////                                               process_io);
//        return rtc::PhysicalSocketServer::Wait(cms == -1 ? 1 : cms,
//                                               process_io);
//    }

//protected:
//    rtc::Thread* thread_;
//    QGuiApplication* wnd_;
//    Conductor* conductor_;
//    PeerConnectionClient* client_;
//};



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


//    rtc::FlagList::SetFlagsFromCommandLine(&argc, argv, true);
//    if (FLAG_help) {
//        rtc::FlagList::Print(NULL, false);
//        return 0;
//    }

//    // Abort if the user specifies a port that is outside the allowed
//    // range [1, 65535].
//    if ((FLAG_port < 1) || (FLAG_port > 65535)) {
//        printf("Error: %i is not a valid port.\n", FLAG_port);
//        return -1;
//    }

    PeerClientCallback cb(&a);

    MainWindow w;
    w.show();
    w.RegisterObserver(&cb);

    AlConnClient cl;
    w.setWsClient(&cl);
    // **
    // * WS_CLIENT-MAIN_WINDOW
    // *
    QObject::connect(&cl, SIGNAL(OnPeerConnectedSignal(QString, QString)),
                     &w, SLOT(OnPeerConnectedSlot(QString, QString)));

    int execReturn = 0;

//    // **
//    // * CUSTOM SOCKET
//    // *
//    rtc::AutoThread auto_thread;
//    rtc::Thread* thread = rtc::Thread::Current();
//    CustomSocketServer socket_server(thread, &a);
//    thread->set_socketserver(&socket_server);

//    rtc::InitializeSSL();
//    // Must be constructed after we set the socketserver.
//    PeerConnectionClient client;
//    rtc::scoped_refptr<Conductor> conductor(
//                new rtc::RefCountedObject<Conductor>(&client, &w));
//    socket_server.set_client(&client);
//    socket_server.set_conductor(conductor);

    // **
    // * WS_CLIENT-CONDUCTOR
    // *
    QObject::connect(&cl, SIGNAL(OnSignedInSignal()), &cb, SLOT(OnSignedInSlot()));

    QObject::connect(&cb, SIGNAL(SendToPeerSignal(QString, const QString&)),
                     &cl, SLOT(SendToPeerSlot(QString, const QString&)));

    QObject::connect(&cb, SIGNAL(SendHangUpSignal(QString)),
                     &cl, SLOT(SendHangUpSlot(QString)));
    // TODO move to client-window interaction
//    QObject::connect(&cl, SIGNAL(OnPeerConnectedSignal(QString, QString)),
//                     conductor, SLOT(OnPeerConnectedSlot(QString, QString)));
    QObject::connect(&cl, SIGNAL(OnMessageFromPeerSignal(std::string, std::string)),
                     &cb, SLOT(OnMessageFromPeerSlot(std::string, std::string)));

    // **
    // * WS_CLIENT-MAIN_WIDOW
    // *
    QObject::connect(&w, SIGNAL(ConnectToPeerSignal(QString)), &cb, SLOT(ConnectToPeerSlot(QString)));




    //======================================
    //======================================
    //======================================
    //test external lib
    void* handle = dlopen("/home/xors/workspace/QT/build-altexo-dev-Release/al_manager/libal_manager.so", RTLD_LAZY);

     AlManager* (*create)();
     void (*destroy)(AlManager*);

     create = (AlManager* (*)())dlsym(handle, "create_object");
     destroy = (void (*)(AlManager*))dlsym(handle, "destroy_object");

     AlManager* myClass = (AlManager*)create();
     cb.setManager(myClass);
     myClass->init(&cb);
     myClass->run();

     std::cout << "STOPPED" << std::endl;
     destroy( myClass );
    //~ test external lib
    //======================================
    //======================================
    //======================================
//    thread->Run();

//    thread->set_socketserver(NULL);
//    // TODO(henrike): Run the Gtk main loop to tear down the connection.
//    /*
//     while (gtk_events_pending()) {
//       gtk_main_iteration();
//     }
//     */
//    rtc::CleanupSSL();

////    // As the render threads make use of our QGuiApplication object
////    // to clean up gracefully, wait for them to finish before
////    // QGuiApp is taken off the heap.
////    for (int i = 0; i < ThreadRenderer::threads.count(); i++) {
//////    foreach (QThread *t, ThreadRenderer::threads) {
////        QThread *t = ThreadRenderer::threads[i];
////        t->wait();
////        delete t;
////    }
    execReturn  = a.exec();
    return execReturn;
}
