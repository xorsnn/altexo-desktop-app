#ifndef ALMANAGERINTERFACE_H
#define ALMANAGERINTERFACE_H

#include <string>

class AlCallback;
class AlManager;

class AlManagerInterface
{
public:
    enum DesiredVideoSource {
        IMG_SNAPSHOTS = 1,
        CAMERA,
    };

    AlManagerInterface();
    ~AlManagerInterface();
    virtual void init(AlCallback* alCallback);
    virtual void run();

    virtual bool hasConnections();
    virtual void InitializePeerConnection();
    virtual void DeletePeerConnection();

    virtual void OnMessageFromPeer(std::string peer_id, const std::string& message);

    //new
    virtual bool isClientConnected();
    virtual void clientConnect(const std::string& server, int port,
                    const std::string& client_name);
    virtual void clientSignOut();

    virtual void setDesiredDataSource(int dataSource);
    virtual void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);

private:
    AlManager* m_manager;

};

#endif // ALMANAGERINTERFACE_H
