#include "alvideoappsrc.h"

ALVideoAppSrc::ALVideoAppSrc(QObject *parent) :
    QObject(parent)
{
}

void ALVideoAppSrc::needData(uint length)
{
    Q_UNUSED(length);

//    qDebug() << "VideoAppSrc NEED DATA. Length:" << length;

    if (!buffer.isNull())
    {
        buffer.clear();
    }
    buffer = QGst::Buffer::create(1280*480*3);
    buffer->map(mapInfo, QGst::MapWrite);

    emit sigNeedData(buffer->size(), (char*)mapInfo.data());//deprecated
    emit this->needDataSignal();
}

void ALVideoAppSrc::enoughData()
{
//    qDebug() << "VideoAppSrc ENOUGH DATA";
}

void ALVideoAppSrc::pushVideoBuffer()
{
    if (buffer.isNull())
    {
        return;
    }

    buffer->unmap(mapInfo);

//    qDebug() << "VideoAppSrc PUSHBUFFER Length:" << buffer->size();
    pushBuffer(buffer);
}

void ALVideoAppSrc::newFrameSlot(QImage image) {
    std::copy (image.bits(), image.bits()+(1280*480*3), mapInfo.data());
    this->pushVideoBuffer();
}
