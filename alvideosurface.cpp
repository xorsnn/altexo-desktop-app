#include "alvideosurface.h"

ALVideoSurface::ALVideoSurface(VideoSurfaceImageObserver *mObserver, QObject *parent) :
    QAbstractVideoSurface(parent)
{
    observer = mObserver;
    m_imageFormat = QImage::Format_Invalid;
}


/**
*Данный метод определяет, какие форматы видео фреймов
*поддерживаются нашим video surface'ом
*для Symbian данного списка форматов будет достаточно
*/
QList<QVideoFrame::PixelFormat> ALVideoSurface::supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

/**
*Наш самый важный метод.
*Здесь, получая фрейм с камеры в объекте QVideoFrame,
*мы должны создать из него QImage и передать в колбэк
*/
bool ALVideoSurface::present(const QVideoFrame &frame){
    m_frame = frame;
    //Проверяем "на вшивость" - соответствует ли текущий формат входящего фрейма
    //формату, установленному для текущего surface'а.
    //В нашем случае текущий формат устанавливает сама камера,
    //так что по идее будет соответствовать всегда
    if(surfaceFormat().pixelFormat() != m_frame.pixelFormat() ||
        surfaceFormat().frameSize() != m_frame.size()) {
        stop();
        return false;
    } else {
        //Создаем из фрейма QImage...
        if (m_frame.map(QAbstractVideoBuffer::ReadOnly)) {
         QImage image(
                 m_frame.bits(),
                 m_frame.width(),
                 m_frame.height(),
                 m_frame.bytesPerLine(),
                 m_imageFormat);

         //... и передаем его в колбэк
         observer->newImage(image);
        }
        return true;
    }
}
