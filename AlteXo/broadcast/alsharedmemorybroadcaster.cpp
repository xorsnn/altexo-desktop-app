#include "alsharedmemorybroadcaster.h"
#include <QDebug>
#include <QBuffer>


AlSharedMemoryBroadcaster::AlSharedMemoryBroadcaster(QObject *parent) : QObject(parent), sharedMemory("QSharedMemoryExample")
{
    qDebug() << "constructor";
    this->sharedMemory.detach();
}

void AlSharedMemoryBroadcaster::slotNewFrame(QImage img) {

//    img.save("out.png");
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << img.convertToFormat(QImage::Format_RGB888);
    int size = buffer.size();

    if (!this->sharedMemory.isAttached()) {
        if (!this->sharedMemory.create(size)) {
            qDebug() << "Can't create!!!!";
            this->sharedMemory.detach();
            return;
        }
        qDebug() << "NOT ATTACHED";
    }

    this->sharedMemory.lock();
    char *to = (char*)this->sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(this->sharedMemory.size(), size));
    this->sharedMemory.unlock();
}
