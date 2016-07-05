#ifndef ALWSPLUGIN_H
#define ALWSPLUGIN_H

#include "AL_API/ws_client_api.hpp"
#include "alconnclient.hpp"
#include <boost/config.hpp>
#include <string>

class AlCallback;
class AlManager;

class AlWsPlugin : public AlWsClientInterface {
public:
  AlWsPlugin();
  ~AlWsPlugin();

  void init(AlCallback *alCallback);

private:
  AlConnClient *m_connClient;
};

extern "C" BOOST_SYMBOL_EXPORT AlWsPlugin plugin;
AlWsPlugin plugin;

#endif // ALWSPLUGIN_H
