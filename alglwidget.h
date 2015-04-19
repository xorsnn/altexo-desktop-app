// alglwidget.h

#ifndef ALGLWIDGET_H
#define ALGLWIDGET_H

#include <QGLWidget>
#include <QByteArray>

class ALGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit ALGLWidget(QWidget *parent = 0);
    ~ALGLWidget();
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    QByteArray getFrame();


signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;
    uint SCREEN_WIDTH = 1280;
    uint SCREEN_HEIGHT = 480;
    unsigned char *output = new unsigned char[1280 * 480 * 4];
};

#endif // ALGLWIDGET_H

