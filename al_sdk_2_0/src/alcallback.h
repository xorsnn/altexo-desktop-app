#ifndef ALCALLBACK_H
#define ALCALLBACK_H

#include <map>
#include <vector>

typedef std::map<int, std::string> Peers;

class AlCallback {
public:
  // virtual void sendToPeerCb(const std::string &message) = 0;
  enum SdkMessageType {
    SDP_SM = 1,
    CANDIDATE_SM,
    INIT_SM,
    NEW_FRAME_SM,
    UPDATE_RESOLUTION_SM,
    QUIT_SM,
  };

  // TODO migrate to this 2
  virtual void onSdpCb(const std::string &message) = 0;
  virtual void onCandidateCb(const std::string &message) = 0;

  virtual void sendHangUpCb(std::string peer_id) = 0;
  virtual void dequeueMessagesFromPeerCb() = 0;
  virtual void stopLocalRendererCb() = 0;
  virtual void stopRemoteRendererCb() = 0;
  virtual void ensureStreamingUICb() = 0;
  //    virtual void startRemoteRendererCb(webrtc::VideoTrackInterface*) = 0;
  //    virtual void startLocalRendererCb(webrtc::VideoTrackInterface*) = 0;
  virtual void queueUIThreadCallbackCb(int msg_id, void *data) = 0;
  virtual void switchToPeerListCb(const Peers &peers) = 0;
  virtual void onDisconnectedCb() = 0;
  virtual void processUiEventsCb() = 0;
  virtual void SwitchToStreamingUI() = 0; // ????

  virtual void
  onDevicesListChangedCb(std::vector<std::string> device_names) = 0;

  virtual void updateFrameCb(const uint8_t *image, int width, int height) = 0;
  virtual void updateLocalFrameCb(const uint8_t *image, int width,
                                  int height) = 0;
  virtual std::vector<uint8_t> *getFrameRef() = 0;

  //    TODO move to AlManager
  virtual std::string getVideoDeviceName() = 0;

  virtual bool ifNewMessage() = 0;
  virtual std::pair<int, std::vector<char>> degueueMessage() = 0;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
};

#endif // ALCALLBACK_H
