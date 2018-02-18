#ifndef ALVIDEORENDERER_H
#define ALVIDEORENDERER_H

#include "api/mediastreaminterface.h"
#include "api/peerconnectioninterface.h"

#include "alcallback.h"

class AlVideoRenderer : public rtc::VideoSinkInterface<webrtc::VideoFrame> {
public:
  enum RendererType {
    LOCAL = 1,
    REMOTE,
  };

  AlVideoRenderer(const int target,
                  webrtc::VideoTrackInterface *track_to_render,
                  AlCallback *alCallback, RendererType rendererType);
  virtual ~AlVideoRenderer();

  // VideoSinkInterface implementation
  void OnFrame(const webrtc::VideoFrame &frame) override;
  const uint8_t *image() const { return m_image.get(); }
  int width() const { return m_width; }
  int height() const { return m_height; }

protected:
  void SetSize(int width, int height);
  void handleCallback();
  std::unique_ptr<uint8_t> m_image;
  int m_width;
  int m_height;
  rtc::scoped_refptr<webrtc::VideoTrackInterface> m_rendered_track;
  const int m_nameReceiver;
  AlCallback *m_alCallback;
  RendererType m_type;
};

#endif // ALVIDEORENDERER_H
