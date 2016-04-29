#ifndef ALDATAMANAGER_H
#define ALDATAMANAGER_H

#include "talk/app/webrtc/mediastreaminterface.h"
#include "alvideorenderer.h"

//#include "talk/app/webrtc/mediastreaminterface.h"
//#include "peer_connection_client.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"

class AlDataManager
{
public:
    AlDataManager();
    void startRemoteRenderer(webrtc::VideoTrackInterface*);
private:
    //TODO implement local renderer later
//    rtc::scoped_ptr<VideoRenderer> m_localRenderer;
    rtc::scoped_ptr<AlVideoRenderer> m_remoteRenderer;
};

#endif // ALDATAMANAGER_H
