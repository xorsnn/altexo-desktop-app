#include "aldatamanager.h"
#include <iostream>
AlDataManager::AlDataManager()
{

}

void AlDataManager::startRemoteRenderer(webrtc::VideoTrackInterface* remote_video, AlCallback* alCallback) {
    std::cout << "AlDataManager::startRemoteRenderer" << std::endl;
    m_remoteRenderer.reset(new AlVideoRenderer(1, remote_video, alCallback));
}
