#include "alvideosurface.h"

ALVideoSurface::ALVideoSurface(QWidget *parent) :
    QWidget(parent)
{
}

void ALVideoSurface::newFrameSlot(QImage image) {
    qDebug() << "new frame arrived";
    this->img = image.scaled(640, 240);
    this->update();
}

void ALVideoSurface::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    //Заливаем видоискатель черным.
    //Зачем - опишу ниже.
    painter.fillRect(this->geometry(), Qt::black);

    //Определяем область рисования и...
    QRect pixR(this->img.rect());
    pixR.moveCenter(this->geometry().center());

    //... рисуем!
    painter.save();
    painter.drawImage(pixR, this->img);
    painter.restore();

}
