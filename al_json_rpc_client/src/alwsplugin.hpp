#ifndef ALWSPLUGIN_H
#define ALWSPLUGIN_H

#include "AL_API/ws_client_api.hpp"
#include "AL_CB/al_ws_cb.hpp"
#include "alconnclient.hpp"
#include "altextmessage.hpp"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <string>

class AlManager;

class AlWsPlugin : public AlWsAPI {
public:
  AlWsPlugin();
  ~AlWsPlugin();

  // api
  void init(AlWsCb *alWsCb);

  // TODO: deprecated
  void sendMessageToPeer(AlTextMessage peerId, AlTextMessage msg);
  void sendMessage(AlTextMessage msg);

  void sendSdpOffer(AlTextMessage msg);
  void sendSdpAnswer(AlTextMessage msg);
  void sendIceCandidate(AlTextMessage msg);

  // json-rpc
  void roomOpen(AlTextMessage msg);

  void threadMain();

private:
  AlConnClient *m_connClient;
  boost::thread m_internalThread;

  // TODO: deprecated
  boost::signals2::signal<void(std::vector<char>, std::vector<char>)>
      sendWsMessageToPeerSignal;

  // TODO: deprecated
  boost::signals2::signal<void(AlTextMessage)> sendWsMessageSignal;

  boost::signals2::signal<void(AlTextMessage)> sendSdpOfferSignal;
  boost::signals2::signal<void(AlTextMessage)> sendSdpAnswerSignal;
  boost::signals2::signal<void(AlTextMessage)> sendIceCandidateSignal;

  // json-rpc
  boost::signals2::signal<void(AlTextMessage)> roomOpenSignal;
};

extern "C" BOOST_SYMBOL_EXPORT AlWsPlugin plugin;
AlWsPlugin plugin;

#endif // ALWSPLUGIN_H
