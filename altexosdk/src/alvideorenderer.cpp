#include "alvideorenderer.h"
#include "api/video/i420_buffer.h"
#include "libyuv/convert_from.h"

AlVideoRenderer::AlVideoRenderer(const int nameReceiver,
                                 webrtc::VideoTrackInterface *track_to_render,
                                 AlCallback *alCallback,
                                 RendererType rendererType)
    : m_width(0), m_height(0), m_rendered_track(track_to_render),
      m_nameReceiver(nameReceiver), m_alCallback(alCallback),
      m_type(rendererType) {
  m_rendered_track->AddOrUpdateSink(this, rtc::VideoSinkWants());
}

AlVideoRenderer::~AlVideoRenderer() { m_rendered_track->RemoveSink(this); }

void AlVideoRenderer::SetSize(int width, int height) {
  if (m_width == width && m_height == height) {
    return;
  }
  m_width = width;
  m_height = height;
  m_image.reset(new uint8_t[width * height * 3]);
}

void AlVideoRenderer::OnFrame(const webrtc::VideoFrame &video_frame) {
  rtc::scoped_refptr<webrtc::I420BufferInterface> buffer(
      video_frame.video_frame_buffer()->ToI420());

  SetSize(buffer->width(), buffer->height());

  libyuv::I420ToRGB24(buffer->DataY(), buffer->StrideY(), buffer->DataU(),
                      buffer->StrideU(), buffer->DataV(), buffer->StrideV(),
                      m_image.get(), m_width * 3, buffer->width(),
                      buffer->height());
  handleCallback();
}

void AlVideoRenderer::handleCallback() {
  switch (m_type) {
  case LOCAL: {
    m_alCallback->updateLocalFrameCb(image(), width(), height());
    break;
  };
  case REMOTE: {
    m_alCallback->updateFrameCb(image(), width(), height());
    break;
  };
  }
}
