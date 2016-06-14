#include "aldatamanager.h"
#include <iostream>
AlDataManager::AlDataManager():
    m_debug(true),
    m_alVideoCapturer(NULL)
{
    m_desiredVideoSource = 1;
}

void AlDataManager::startRemoteRenderer(webrtc::VideoTrackInterface* remote_video, AlCallback* alCallback) {
    std::cout << "AlDataManager::startRemoteRenderer" << std::endl;
    m_remoteRenderer.reset(new AlVideoRenderer(1, remote_video, alCallback));
}

void AlDataManager::setDesiredDataSource(int dataSource) {
    std::cout << "AlDataManager::setDesiredDataSource" << std::endl;
    m_desiredVideoSource = dataSource;
}

void AlDataManager::startVideoCapturer(AlVideoCapturer *alVideoCapturer) {
    m_alVideoCapturer = alVideoCapturer;
}

void AlDataManager::setImageData(uint8_t *pImageBytes, size_t len, int width, int height) {
    if (m_alVideoCapturer) {
        m_alVideoCapturer->setImageData(pImageBytes, len, width, height);
    }
}
