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

signals:

    void sigNeedData(uint length, char* data);//deprecated
    void needDataSignal();

public slots:
    void newFrameSlot(QImage image);
    void pushVideoBuffer();

private:
    QGst::BufferPtr buffer;
    QGst::MapInfo mapInfo;

};

#endif // ALVIDEOAPPSRC_H
