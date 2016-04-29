#include "alvideorenderer.h"
#include <iostream>


AlVideoRenderer::AlVideoRenderer(const int nameReceiver,
                                 webrtc::VideoTrackInterface* track_to_render)
    : m_width(0),
      m_height(0),
      m_rendered_track(track_to_render),
      m_nameReceiver(nameReceiver) {
  m_rendered_track->AddRenderer(this);
}

AlVideoRenderer::~AlVideoRenderer() {
  m_rendered_track->RemoveRenderer(this);
}


void AlVideoRenderer::SetSize(int width, int height) {
    std::cout << "AlVideoRenderer::SetSize" << std::endl;
//  gdk_threads_enter();

  if (m_width == width && m_height == height) {
    return;
  }

  m_width = width;
  m_height = height;
  m_image.reset(new uint8_t[width * height * 4]);
//  gdk_threads_leave();
}

void AlVideoRenderer::RenderFrame(
    const cricket::VideoFrame* video_frame) {
    std::cout << "AlVideoRenderer::RenderFrame" << std::endl;
//  gdk_threads_enter();

//  const cricket::VideoFrame* frame = video_frame->GetCopyWithRotationApplied();

//  SetSize(static_cast<int>(frame->GetWidth()),
//          static_cast<int>(frame->GetHeight()));

//  int size = m_width * m_height * 4;
//  // TODO(henrike): Convert directly to RGBA
//  frame->ConvertToRgbBuffer(cricket::FOURCC_ARGB,
//                            image_.get(),
//                            size,
//                            m_width * 4);
//  // Convert the B,G,R,A frame to R,G,B,A, which is accepted by GTK.
//  // The 'A' is just padding for GTK, so we can use it as temp.
//  uint8_t* pix = image_.get();
//  uint8_t* end = image_.get() + size;
//  while (pix < end) {
//    pix[3] = pix[0];     // Save B to A.
//    pix[0] = pix[2];  // Set Red.
//    pix[2] = pix[3];  // Set Blue.
//    pix[3] = 0xFF;     // Fixed Alpha.
//    pix += 4;
//  }

//  gdk_threads_leave();

//  g_idle_add(Redraw, main_wnd_);
}
