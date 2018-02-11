#ifndef ALWSPLUGIN_H
#define ALWSPLUGIN_H

#include "AL_API/ws_client_api.hpp"
#include "AL_CB/al_ws_cb.hpp"
#include "alconnclient.hpp"
#include "boost/thread.hpp"
#include "altextmessage.hpp"

#ifdef _WIN32
#include "boost/dll/alias.hpp"
#endif

#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <string>

class AlManager;

class AlWsPlugin : public AlWsAPI {
public:
  AlWsPlugin();
  ~AlWsPlugin();

#ifdef _WIN32
  // Factory method
  static boost::shared_ptr<AlWsAPI> create() {
    return boost::make_shared<AlWsPlugin>();
  }
#endif

  // api
  void init(AlWsCb *alWsCb);

  // TODO: deprecated
  void sendMessageToPeer(AlTextMessage peerId, AlTextMessage msg);
  void sendMessage(AlTextMessage msg);

  void sendSdpOffer(const char *cMsg);
  void sendSdpAnswer(AlTextMessage msg);
  void sendIceCandidate(AlTextMessage msg);

  // json-rpc
  void roomOpen(AlTextMessage msg);
  void userMode(al::VideoMode videoMode);

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
  boost::signals2::signal<void(al::VideoMode)> userModeSignal;

  // json-rpc
  boost::signals2::signal<void(AlTextMessage)> roomOpenSignal;
};

#ifdef _WIN32
BOOST_DLL_ALIAS(AlWsPlugin::create, // <-- this function is exported with...
                create_plugin        // <-- ...this alias name
                )
#else
extern "C" BOOST_SYMBOL_EXPORT AlWsPlugin plugin;
AlWsPlugin plugin;
#endif

#endif // ALWSPLUGIN_H
