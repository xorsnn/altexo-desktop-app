
#ifndef ALRPC_H
#define ALRPC_H

#include "alrpcrequest.hpp"
#include "altextmessage.hpp"
#include "alwsclient.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <map>

class AlRpc : public AlWsClient {
public:
  enum ROOM_TYPE { ONE_2_ONE = 1, ONE_2_MANY };

  AlRpc();
  ~AlRpc();
  long getMsgId();

  void authenticate(std::string token);
  void roomOpen(AlTextMessage roomName);
  void roomClose();
  void roomEnter();
  void roomLeave();
  void roomOffer(AlTextMessage offerSdp);

  void onRoomIceCandidate();
  void offer();
  void onIceCandidate();

  // TODO: message from server endpoint
  void onMessage(AlTextMessage msg);

  void sendSdpOffer(AlTextMessage msg);
  void sendSdpAnswer(AlTextMessage msg);
  void sendIceCandidate(AlTextMessage msg);

private:
  long m_sessionMsgCounter;
  std::map<long, int> m_requests;
  long m_sdpOfferId;

  std::string m_curRoomName;
};

#endif // ALRPC_H
