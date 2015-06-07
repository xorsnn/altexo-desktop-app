// alglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>

#include "alglwidget.h"

ALGLWidget::ALGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
//    xRot = 0;
//    yRot = 0;
//    zRot = 0;

    this->interface = new ALKinectInterface();
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimerFired()));
    timer->start(33);
}

ALGLWidget::~ALGLWidget()
{
}

ALKinectInterface* ALGLWidget::getALKinectInterface() {
    return this->interface;
}

void ALGLWidget::updateTimerFired() {
    update();
}

QSize ALGLWidget::minimumSizeHint() const
{
    return QSize(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
}

QSize ALGLWidget::sizeHint() const
{
    return QSize(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
}

//static void qNormalizeAngle(int &angle)
//{
//    while (angle < 0)
//        angle += 360 * 16;
//    while (angle > 360)
//        angle -= 360 * 16;
//}

//void ALGLWidget::setXRotation(int angle)
//{
//    qNormalizeAngle(angle);
//    if (angle != xRot) {
//        xRot = angle;
//        emit xRotationChanged(angle);
//        updateGL();
//    }
//}

//void ALGLWidget::setYRotation(int angle)
//{
//    qNormalizeAngle(angle);
//    if (angle != yRot) {
//        yRot = angle;
//        emit yRotationChanged(angle);
//        updateGL();
//    }
//}

//void ALGLWidget::setZRotation(int angle)
//{
//    qNormalizeAngle(angle);
//    if (angle != zRot) {
//        zRot = angle;
//        emit zRotationChanged(angle);
//        updateGL();
//    }
//}

void ALGLWidget::ReSizeGLScene(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, 1280, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void ALGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    //old (
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

//    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //~old)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &gl_depth_tex);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &gl_rgb_tex);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ReSizeGLScene(this->SCREEN_WIDTH, this->SCREEN_HEIGHT);

}

void ALGLWidget::paintGL()
{
    //old
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//    glTranslatef(0.0, 0.0, -10.0);
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
//    draw();
    //~old
    qglColor(Qt::red);
    static std::vector<uint8_t> depth(640*480*3);
    static std::vector<uint8_t> rgb(640*480*3);
    this->interface->updateDeviceState();
    this->interface->getDepthDt(depth);
    this->interface->getRGBDt(rgb);
//    thisObject->device->getDepth(depth);
//    thisObject->device->getRGB(rgb);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, this->gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 1); glVertex3f(640,480,0);
    glTexCoord2f(0, 1); glVertex3f(0,480,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, this->gl_rgb_tex);
//    if (thisObject->device->getVideoFormat() == FREENECT_VIDEO_RGB || thisObject->device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB)
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
//    else
//        glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
    glTexCoord2f(0, 1); glVertex3f(640,480,0);
    glEnd();
    glReadBuffer(GL_BACK);

    mutex.lock();
    glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, this->output);
    mutex.unlock();

    swapBuffers();
    //make it if new buffer

//    updateGL();
//    update();
//    glFlush();
//    glutSwapBuffers();
}

void ALGLWidget::resizeGL(int width, int height)
{
//    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
//#else
//    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
//#endif
//    glMatrixMode(GL_MODELVIEW);
    this->ReSizeGLScene(width, height);
}

//void ALGLWidget::mousePressEvent(QMouseEvent *event)
//{
//    lastPos = event->pos();
//}

//void ALGLWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    int dx = event->x() - lastPos.x();
//    int dy = event->y() - lastPos.y();

//    if (event->buttons() & Qt::LeftButton) {
//        setXRotation(xRot + 8 * dy);
//        setYRotation(yRot + 8 * dx);
//    } else if (event->buttons() & Qt::RightButton) {
//        setXRotation(xRot + 8 * dy);
//        setZRotation(zRot + 8 * dx);
//    }

//    lastPos = event->pos();
//}

//void ALGLWidget::draw()
//{
//    qglColor(Qt::red);
//    glBegin(GL_QUADS);
//    glNormal3f(0,0,-1);
//    glVertex3f(-1,-1,0);
//    glVertex3f(-1,1,0);
//    glVertex3f(1,1,0);
//    glVertex3f(1,-1,0);

//    glEnd();
//    glBegin(GL_TRIANGLES);
//    glNormal3f(0,-1,0.707);
//    glVertex3f(-1,-1,0);
//    glVertex3f(1,-1,0);
//    glVertex3f(0,0,1.2);
//    glEnd();
//    glBegin(GL_TRIANGLES);
//    glNormal3f(1,0, 0.707);
//    glVertex3f(1,-1,0);
//    glVertex3f(1,1,0);
//    glVertex3f(0,0,1.2);
//    glEnd();
//    glBegin(GL_TRIANGLES);
//    glNormal3f(0,1,0.707);
//    glVertex3f(1,1,0);
//    glVertex3f(-1,1,0);
//    glVertex3f(0,0,1.2);
//    glEnd();
//    glBegin(GL_TRIANGLES);
//    glNormal3f(-1,0,0.707);
//    glVertex3f(-1,1,0);
//    glVertex3f(-1,-1,0);
//    glVertex3f(0,0,1.2);
//    glEnd();

//    //xors

//    //~xors
//}

QByteArray ALGLWidget::getFrame() {
    QByteArray res(reinterpret_cast<const char*>(this->output), this->SCREEN_WIDTH*this->SCREEN_HEIGHT*4);
    return res;
}

void ALGLWidget::ALUpdateGL() {
    updateGL();
}

void ALGLWidget::bufferFill(quint8 *buffer, uint size) {
    mutex.lock();
    qDebug() << "start copy";
    std::copy (this->output, this->output+size, buffer);
    qDebug() << "copied";
    mutex.unlock();
}

void ALGLWidget::gstBufferFill(GstBuffer *buffer, guint size) {
    mutex.lock();
    gst_buffer_fill (buffer, 0, this->output, size);
    mutex.unlock();
}

void ALGLWidget::needDataSlot(uint length, char *data) {

    static std::vector<uint8_t> rgb_(640*480*3);
    this->interface->getRGBDt(rgb_);
    static std::vector<uint8_t> depth_(640*480*3);
    this->interface->getDepthDt(depth_);

    QImage image_rgb((&(rgb_[0])), 640, 480, QImage::Format_RGB888);
//    image_rgb.save("rgb.jpg");

    QImage image_depth((&(depth_[0])), 640, 480, QImage::Format_RGB888);
//    image_depth.save("depth.jpg");

    QImage image(1280, 480, QImage::Format_RGB888);
    QPainter painter;
    painter.begin(&image);
    painter.drawImage(0, 0, image_depth);
    painter.drawImage(640, 0, image_rgb);
    painter.end();
//    image.save("com.jpg",0,100);

//    std::copy (this->output, this->output+length, data);
//    image.convertToFormat(QImage::Format_RGBA8888);
//    image.convertToFormat(QImage::Format_RGB32);
    std::copy (image.bits(), image.bits()+(1280*480*3), data);
    emit this->pushDataSignal();
}
