#ifndef ALVIDEOSURFACE_H
#define ALVIDEOSURFACE_H

#include <QWidget>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include "videosurfaceimageobserver.h"

class ALVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
//    explicit ALVideoSurface(QWidget *parent = 0);
    ALVideoSurface(VideoSurfaceImageObserver *mObserver, QObject *parent = 0);
    bool present(const QVideoFrame &frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type=QAbstractVideoBuffer::NoHandle ) const;

signals:

public slots:
private:
      QVideoFrame m_frame;
      QImage::Format m_imageFormat;
      QVideoSurfaceFormat m_videoFormat;
      VideoSurfaceImageObserver *observer;
};

#endif // ALVIDEOSURFACE_H
