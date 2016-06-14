#ifndef ALDATAMANAGER_H
#define ALDATAMANAGER_H

#include "alvideorenderer.h"
#include "alvideocapturer.h"
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

class AlDataManager
{
public:
    AlDataManager();
    void startRemoteRenderer(webrtc::VideoTrackInterface*,AlCallback*);
    void setDesiredDataSource(int dataSource);
    void startVideoCapturer(AlVideoCapturer* alVideoCapturer);
    void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);
private:
//    TODO implement local renderer later
//    rtc::scoped_ptr<VideoRenderer> m_localRenderer;

    rtc::scoped_ptr<AlVideoRenderer> m_remoteRenderer;
    std::deque<std::string*> m_pendingMessages;
    bool m_debug;

    AlVideoCapturer* m_alVideoCapturer;
public:
    int m_desiredVideoSource;
};

#endif // ALDATAMANAGER_H
