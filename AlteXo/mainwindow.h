#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
#include <QDialogButtonBox>
#include "alvideosurface.h"
#include "settingsdialog.h"

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

public Q_SLOTS:
    ALVideoSurface* getVideoSurfaceRef();
    void requestNewFrameSlot();
    void settingsChangedSlot();

private Q_SLOTS:
    void on_startRecorder_clicked();
    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    ALVideoSurface * videoSurface;
    QTimer *timer;
Q_SIGNALS:
    void settingsChangedSignal();

    void startRecorderSignal();
    void stopRecorderSignal();

    void requestNewFrameSignal();
};

#endif // MAINWINDOW_H
