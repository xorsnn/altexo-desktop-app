#include "alrecorder.h"

ALRecorder::ALRecorder(QObject *parent) :
    QObject(parent)
{
    this->outputFilePath = QDir::currentPath() + QDir::separator() + "ouy.webm";
}

void ALRecorder::startSlot() {
    this->start();
}

void ALRecorder::stopSlot() {
    qDebug() << "stop fired";
    if (this->m_pipeline) { //pipeline exists - destroy it
            //send an end-of-stream event to flush metadata and cause an EosMessage to be delivered
        this->m_pipeline->sendEvent(QGst::EosEvent::create());
    } else {
        this->stop();
    }
}

QGst::BinPtr ALRecorder::createAudioSrcBin() {
    QGst::BinPtr audioBin;

    try {
        audioBin = QGst::Bin::fromDescription("autoaudiosrc name=\"audiosrc\" ! audioconvert ! "
                                              "audioresample ! audiorate ! vorbisenc ! queue");
    } catch (const QGlib::Error & error) {
        qCritical() << "Failed to create audio source bin:" << error;
        return QGst::BinPtr();
    }

    QGst::ElementPtr src = audioBin->getElementByName("audiosrc");
    //autoaudiosrc creates the actual source in the READY state
    src->setState(QGst::StateReady);
    return audioBin;
}

QGst::BinPtr ALRecorder::createVideoSrcBin() {
    QGst::BinPtr videoBin;
    QString rawvideocaps = QString("video/x-raw,format=RGB,width=1280,height=480,framerate=25/1,pixel-aspect-ratio=1/1");
    //live (high compression for online streaming)
//    8192000
//    16384000
//    32768000
    QString outputPipeDesc = QString(" appsrc name=videosrc caps=\"%1\" is-live=true format=time do-timestamp=true ! videorate !"
                                     " videoconvert ! vp8enc threads=2 deadline=1 cpu-used=15 target-bitrate=32768000 ! queue")
            .arg(rawvideocaps);
    try {
        videoBin = QGst::Bin::fromDescription(outputPipeDesc);
        this->m_src.setElement(videoBin->getElementByName("videosrc"));
        return videoBin;
    } catch (const QGlib::Error & error) {
        qCritical() << "Failed to create video source bin:" << error;
        return QGst::BinPtr();
    }
    return videoBin;
}

void ALRecorder::start() {
    QGst::BinPtr audioSrcBin = createAudioSrcBin();
    QGst::BinPtr videoSrcBin = createVideoSrcBin();
    QGst::ElementPtr mux = QGst::ElementFactory::make("webmmux");
    QGst::ElementPtr sink = QGst::ElementFactory::make("filesink");

    if (!audioSrcBin || !videoSrcBin || !mux || !sink) {
//        QMessageBox::critical(this, tr("Error"), tr("One or more elements could not be created. "
//                                                    "Verify that you have all the necessary element plugins installed."));
        qDebug() << tr("One or more elements could not be created. Verify that you have all the necessary element plugins installed.");
        return;
    }

    sink->setProperty("location", this->outputFilePath);

    m_pipeline = QGst::Pipeline::create();
    m_pipeline->add(audioSrcBin, videoSrcBin, mux, sink);

    //link elements
    QGst::PadPtr audioPad = mux->getRequestPad("audio_%u");

    audioSrcBin->getStaticPad("src")->link(audioPad);

    QGst::PadPtr videoPad = mux->getRequestPad("video_%u");
    videoSrcBin->getStaticPad("src")->link(videoPad);

    mux->link(sink);

    //connect the bus
    m_pipeline->bus()->addSignalWatch();
    QGlib::connect(m_pipeline->bus(), "message", this, &ALRecorder::onBusMessage);

    //go!
    m_pipeline->setState(QGst::StatePlaying);
    this->statusText = tr("Stop recording");
}

void ALRecorder::stop() {
    //stop recording
    m_pipeline->setState(QGst::StateNull);

    //clear the pointer, destroying the pipeline as its reference count drops to zero.
    m_pipeline.clear();

    this->statusText = tr("Start recording");
}

void ALRecorder::onBusMessage(const QGst::MessagePtr & message)
{
    switch (message->type()) {
    case QGst::MessageEos:
        //got end-of-stream - stop the pipeline
//        qDebug() << "got EOS";
        stop();
        break;
    case QGst::MessageError:
        //check if the pipeline exists before destroying it,
        //as we might get multiple error messages
        if (m_pipeline) {

            stop();
        }
        qDebug() << tr("Pipeline Error") << " | " << message.staticCast<QGst::ErrorMessage>()->error().message();
        break;
    default:
        break;
    }
}

