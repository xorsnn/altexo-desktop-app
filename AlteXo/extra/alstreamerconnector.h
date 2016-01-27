#ifndef ALSTREAMERCONNECTOR_H
#define ALSTREAMERCONNECTOR_H

#include <QObject>

class AlStreamerConnector : public QObject
{
    Q_OBJECT
public:
    explicit AlStreamerConnector(QObject* streamingObj = Q_NULLPTR, QObject* conductor = Q_NULLPTR, QObject *parent = Q_NULLPTR) {
        this->streamingObj = streamingObj;
        this->conductor = conductor;
    }

public Q_SLOTS:

    void start() {
        QObject::connect(this->streamingObj, SIGNAL(newWFrameSignal(QImage)), this->conductor, SLOT(slotSetNewFrame(QImage)));
    }

    void setStreamignObj(QObject* streamingObj) {
        this->streamingObj = streamingObj;
    }

    void setConductor(QObject* conductor) {
        this->conductor = conductor;
    }

private:
    QObject* streamingObj;
    QObject* conductor;
};

#endif // ALSTREAMERCONNECTOR_H
