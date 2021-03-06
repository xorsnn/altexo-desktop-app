#ifndef ALSTREAMERINTERFACE_H
#define ALSTREAMERINTERFACE_H

#include <QObject>
#include <QApplication>

class AlStreamerInterface
{
public:
    virtual ~AlStreamerInterface() {}

    virtual void initStreamer(QGuiApplication* app) = 0;
    virtual void runStreamer() = 0;
    virtual QObject* getConductor() = 0;

};


QT_BEGIN_NAMESPACE

#define AlStreamerInterface_iid "org.altexo.AlStreamerInterface"

Q_DECLARE_INTERFACE(AlStreamerInterface, AlStreamerInterface_iid)
QT_END_NAMESPACE

#endif // ALSTREAMERINTERFACE_H
