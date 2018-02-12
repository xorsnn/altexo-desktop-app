#ifndef ALCONDUCTOR_H
#define ALCONDUCTOR_H
#include "alcallback.h"
#include "conductor/conductor.h"
#include "customsocketserver.h"

// class AlConductorCb;

class AlManager {

public:
  AlManager();
  virtual ~AlManager();
  /* use virtual otherwise linker will try to perform static linkage */
  virtual void run(AlCallback *alCallback);

  virtual bool hasConnections();
  virtual void InitializePeerConnection();
  virtual void DeletePeerConnection();

  virtual void OnMessageFromPeer(std::string peer_id,
                                 const std::string &message);
  virtual void setDesiredDataSource(AlSdkAPI::DesiredVideoSource dataSource);

  virtual void setImageData(uint8_t *pImageBytes, size_t len, int width,
                            int height);

  // TODO hide
  CustomSocketServer *m_socketServer;

private:
  rtc::Thread *m_thread;
  rtc::scoped_refptr<Conductor> m_conductor;
};

#endif // ALCONDUCTOR_H
