#ifndef ALVIDEOSURFACE_H
#define ALVIDEOSURFACE_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QDebug>

class ALVideoSurface : public QWidget
{
    Q_OBJECT
public:
    explicit ALVideoSurface(QWidget *parent = 0);

public Q_SLOTS:
    void newFrameSlot(QImage image);
private:
    void paintEvent(QPaintEvent *);
    QImage img;
};

#endif // ALVIDEOSURFACE_H
