#ifndef ALVIDEOAPPSRC_H
#define ALVIDEOAPPSRC_H

#include <QObject>
#include <QByteArray>
#include <QImage>

#include <QGst/Memory>
#include <QGst/Buffer>
#include <QGst/Utils/ApplicationSource>

class ALVideoAppSrc : public QObject, public QGst::Utils::ApplicationSource
{
    Q_OBJECT
public:
    explicit ALVideoAppSrc(QObject *parent = 0);

    virtual void needData (uint length);
    virtual void enoughData();

Q_SIGNALS:
    void needDataSignal();

public Q_SLOTS:
    void newFrameSlot(QImage image);
    void pushVideoBuffer();

private:
    QGst::BufferPtr buffer;
    QGst::MapInfo mapInfo;

};

#endif // ALVIDEOAPPSRC_H
