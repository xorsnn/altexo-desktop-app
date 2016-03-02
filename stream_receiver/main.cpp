#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtGui/QGuiApplication>

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformintegration.h>

#include <QtQuick/QQuickView>

#include "threadrenderer.h"

#include <QDialog>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ThreadRenderer>("SceneGraphRendering", 1, 0, "Renderer");
    int execReturn = 0;

    {
        QQuickView view;

        // Rendering in a thread introduces a slightly more complicated cleanup
        // so we ensure that no cleanup of graphics resources happen until the
        // application is shutting down.
        view.setPersistentOpenGLContext(true);
        view.setPersistentSceneGraph(true);

        view.setResizeMode(QQuickView::SizeRootObjectToView);
        view.setSource(QUrl("qrc:/main.qml"));
        view.show();

        execReturn = app.exec();
    }

    // As the render threads make use of our QGuiApplication object
    // to clean up gracefully, wait for them to finish before
    // QGuiApp is taken off the heap.
    foreach (QThread *t, ThreadRenderer::threads) {
        t->wait();
        delete t;
    }


//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    return ap.exec();
}
