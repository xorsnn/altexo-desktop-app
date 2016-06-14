#ifndef ALCALLBACK_H
#define ALCALLBACK_H

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "talk/app/webrtc/videosourceinterface.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
typedef std::map<int, std::string> Peers;

class AlCallback {
public:
    virtual void sendToPeerCb(std::string peer_id, const std::string &message) = 0;
    virtual void sendHangUpCb(std::string peer_id) = 0;
    virtual void dequeueMessagesFromPeerCb() = 0;
    virtual void stopLocalRendererCb() = 0;
    virtual void stopRemoteRendererCb() = 0;
    virtual void ensureStreamingUICb() = 0;
    virtual void startRemoteRendererCb(webrtc::VideoTrackInterface*) = 0;
    virtual void startLocalRendererCb(webrtc::VideoTrackInterface*) = 0;
    virtual void queueUIThreadCallbackCb(int msg_id, void* data) = 0;
    virtual void switchToPeerListCb(const Peers& peers) = 0;
    virtual void onDisconnectedCb() = 0;
    virtual void processUiEventsCb() = 0;
    virtual void SwitchToStreamingUI() = 0; // ????

    virtual void updateFrameCb(const uint8_t* image, int width, int height);


};

#endif // ALCALLBACK_H
