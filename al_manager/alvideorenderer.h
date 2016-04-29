#ifndef ALVIDEORENDERER_H
#define ALVIDEORENDERER_H

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"

class AlVideoRenderer: public webrtc::VideoRendererInterface
{
public:
    AlVideoRenderer(const int target, webrtc::VideoTrackInterface* track_to_render);
    virtual ~AlVideoRenderer();

    // VideoRendererInterface implementation
    virtual void SetSize(int width, int height);
    virtual void RenderFrame(const cricket::VideoFrame* frame);

    const uint8_t* image() const { return m_image.get(); }

    int width() const {
        return m_width;
    }

    int height() const {
        return m_height;
    }

protected:
    rtc::scoped_ptr<uint8_t[]> m_image;
    int m_width;
    int m_height;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> m_rendered_track;
    const int m_nameReceiver;
};


#endif // ALVIDEORENDERER_H
