#ifndef ALVIDEOAPPSRC_H
#define ALVIDEOAPPSRC_H

#include <QObject>
#include <QByteArray>

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
    void sigNeedData(uint length, char* data);

public slots:
    void pushVideoBuffer();

private:
    QGst::BufferPtr buffer;
    QGst::MapInfo mapInfo;

};

#endif // ALVIDEOAPPSRC_H
