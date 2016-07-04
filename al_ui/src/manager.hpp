#ifndef MANAGER_H
#define MANAGER_H

// #include "alconnclient.hpp"
// #include "ws_client_api.hpp"
// #include <boost/config.hpp>
// #include <string>
#include "sdk_api.hpp"
#include "sensor_api.hpp"
#include "ws_client_api.hpp"
#include <boost/dll/import.hpp> // for import_alias

// class AlCallback;
// class AlManager;

class Manager {
public:
  Manager();
  ~Manager();

  // void init(AlCallback *alCallback);

private:
  void initSensor();
  void initWsConnection();

  boost::shared_ptr<AlWsClientInterface> m_WsClient;
  boost::shared_ptr<AlSensorAPI> m_sensor;
  // AlConnClient *m_connClient;
};

// extern "C" BOOST_SYMBOL_EXPORT AlWsPlugin plugin;
// AlWsPlugin plugin;

#endif // MANAGER_H
