#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
#include "alvideosurface.h"
//#include "alkinectinterface.h"
//#include "alglwidget.h"
//#include <gst/gst.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    ALVideoSurface* getVideoSurfaceRef();
    void requestNewFrameSlot();
private slots:
    void on_startRecorder_clicked();

private:
    Ui::MainWindow *ui;
    ALVideoSurface * videoSurface;
    QTimer *timer;
signals:
    void startRecorderSignal();
    void stopRecorderSignal();

    void requestNewFrameSignal();
};

#endif // MAINWINDOW_H
