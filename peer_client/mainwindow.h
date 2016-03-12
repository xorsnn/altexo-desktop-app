#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <string>
#include <QStringListModel>

#include "talk/app/webrtc/mediastreaminterface.h"
#include "peer_connection_client.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"
//#include "webrtc/base/win32.h"

#include "VideoRenderer.h"

class VideoRenderer;

class MainWndCallback {
public:
    virtual void StartLogin(const std::string& server, int port) = 0;
    virtual void DisconnectFromServer() = 0;
    virtual void ConnectToPeer(QString peer_id) = 0;
    virtual void DisconnectFromCurrentPeer() = 0;
    virtual void UIThreadCallback(int msg_id, void* data) = 0;
    virtual void Close() = 0;
protected:
    virtual ~MainWndCallback() {}
};

// Pure virtual interface for the main window.
class MainWnd {
public:
    virtual ~MainWnd() {}

    enum UI {
        CONNECT_TO_SERVER,
        LIST_PEERS,
        STREAMING,
    };

    virtual void RegisterObserver(MainWndCallback* callback) = 0;

    virtual bool IsWindow() = 0;
    virtual void MessageBox(const char* caption, const char* text,
                            bool is_error) = 0;

    virtual UI current_ui() = 0;

    virtual void SwitchToConnectUI() = 0;
    virtual void SwitchToPeerList(const Peers& peers) = 0;
    virtual void SwitchToStreamingUI() = 0;

    virtual void StartLocalRenderer(webrtc::VideoTrackInterface* local_video) = 0;
    virtual void StopLocalRenderer() = 0;
    virtual void StartRemoteRenderer(
            webrtc::VideoTrackInterface* remote_video) = 0;
    virtual void StopRemoteRenderer() = 0;

    virtual void QueueUIThreadCallback(int msg_id, void* data) = 0;
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MainWnd
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // **
    // * MainWnd implementation
    // *

    virtual void RegisterObserver(MainWndCallback* callback);

    virtual bool IsWindow();
    virtual void MessageBox(const char* caption, const char* text,
                            bool is_error);

    virtual MainWindow::UI current_ui();

    virtual void SwitchToConnectUI();
    virtual void SwitchToPeerList(const Peers& peers);
    virtual void SwitchToStreamingUI();

    virtual void StartLocalRenderer(webrtc::VideoTrackInterface* local_video);
    virtual void StopLocalRenderer();
    virtual void StartRemoteRenderer(
            webrtc::VideoTrackInterface* remote_video);
    virtual void StopRemoteRenderer();

    virtual void QueueUIThreadCallback(int msg_id, void* data);

public Q_SLOTS:
    void updateRenderImage(const int target,QImage *newImage);

private Q_SLOTS:
    void on_startButton_clicked();



//protected:
//    class VideoRenderer : public webrtc::VideoRendererInterface {
//    public:
//        VideoRenderer(QMainWindow* main_wnd,
//                      webrtc::VideoTrackInterface* track_to_render);
//        virtual ~VideoRenderer();

//        // VideoRendererInterface implementation
//        virtual void SetSize(int width, int height);
//        virtual void RenderFrame(const cricket::VideoFrame* frame);

//        const uint8_t* image() const { return image_.get(); }

//        int width() const {
//            return width_;
//        }

//        int height() const {
//            return height_;
//        }

//    protected:
//        rtc::scoped_ptr<uint8_t[]> image_;
//        int width_;
//        int height_;
//        QMainWindow* main_wnd_;
//        rtc::scoped_refptr<webrtc::VideoTrackInterface> rendered_track_;
//    };

private:
    Ui::MainWindow *ui;
    MainWndCallback *callback_;
    bool debug_;
    std::string server_;
    std::string port_;

    rtc::scoped_ptr<VideoRenderer> local_renderer_;
    rtc::scoped_ptr<VideoRenderer> remote_renderer_;
    rtc::scoped_ptr<uint8_t[]> draw_buffer_;
    int draw_buffer_size_;

    QStringListModel* m_model;
};

#endif // MAINWINDOW_H
