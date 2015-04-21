#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QByteArray>
#include "alkinectinterface.h"
#include <gst/gst.h>
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

//    void setKinectDeviceInterface(ALKinectInterface * alKinectInterface);

    QByteArray getFrame();
    void gstBufferFill(GstBuffer *buffer, guint size);


private slots:
    void on_startRecorder_clicked();
    void on_stopRecorder_clicked();

private:
    Ui::MainWindow *ui;



signals:
    void startRecorder();
    void stopRecorder();
};

#endif // MAINWINDOW_H
