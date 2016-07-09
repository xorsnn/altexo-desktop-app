#ifndef ALWSPLUGIN_H
#define ALWSPLUGIN_H

#include "AL_API/ws_client_api.hpp"
#include "AL_CB/al_ws_cb.hpp"
#include "alconnclient.hpp"
#include <boost/config.hpp>
#include <boost/signals2/signal.hpp>
#include <string>

class AlManager;

class AlWsPlugin : public AlWsAPI {
public:
  AlWsPlugin();
  ~AlWsPlugin();

  void init(AlWsCb *alWsCb);
  void threadMain();

private:
  AlConnClient *m_connClient;
  boost::thread m_internalThread;

  boost::signals2::signal<void(std::string, std::string)> newMessageSignal;
};

extern "C" BOOST_SYMBOL_EXPORT AlWsPlugin plugin;
AlWsPlugin plugin;

#endif // ALWSPLUGIN_H
