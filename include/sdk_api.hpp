#ifndef ALSDKINTERFACE_H
#define ALSDKINTERFACE_H

#include <string>

class AlCallback;
class AlManager;

class AlSdkInterface
{
public:
    enum DesiredVideoSource {
        IMG_SNAPSHOTS = 1,
        CAMERA,
    };

    // AlSdkInterface();
    virtual ~AlSdkInterface() {}

    virtual void init(AlCallback* alCallback) = 0;
    virtual void run() = 0;
    virtual bool hasConnections() = 0;
    virtual void InitializePeerConnection() = 0;
    virtual void DeletePeerConnection() = 0;
    virtual void OnMessageFromPeer(std::string peerId, const std::string& message) = 0;
    virtual bool isClientConnected() = 0;
    virtual void clientConnect(const std::string& server, int port,
                    const std::string& clientName) = 0;
    virtual void clientSignOut() = 0;
    virtual void setDesiredDataSource(int dataSource) = 0;
    virtual void setImageData(uint8_t *pImageBytes, size_t len, int width, int height) = 0;
};
//]

#endif // ALSDKINTERFACE_H
