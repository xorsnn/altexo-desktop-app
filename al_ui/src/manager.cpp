#include "manager.hpp"
#include <iostream>

Manager::Manager() {
  std::cout << "Manager constructor" << std::endl;
  initWsConnection();
  initSensor();
}

Manager::~Manager() {}

void Manager::initSensor() {

  boost::filesystem::path lib_path(
      "/home/xors/workspace/QT/altexo/al_kinect/build/");
  // boost::shared_ptr<AlWsClientInterface>
  //     plugin; // variable to hold a pointer to plugin variable
  std::cout << "Loading sensor plugin" << std::endl;

  m_sensor = boost::dll::import<AlSensorAPI>( // type of imported
                                              // symbol
                                              // is located
                                              // between `<` and
                                              // `>`
      lib_path / "libal_kinect_1.so", // path to the library and library name
      "plugin",                       // name of the symbol to import
      boost::dll::load_mode::append_decorations // makes
                                                //     `libmy_plugin_sum.so`
                                                // or
                                                // `my_plugin_sum.dll` from
                                                // `my_plugin_sum`
      );

  m_sensor->init(NULL);
}

void Manager::initWsConnection() {
  boost::filesystem::path lib_path(
      "/home/xors/workspace/QT/altexo/al_ws_client/build/");
  // boost::shared_ptr<AlWsClientInterface>
  //     plugin; // variable to hold a pointer to plugin variable
  std::cout << "Loading ws plugin" << std::endl;

  m_WsClient = boost::dll::import<AlWsClientInterface>( // type of imported
                                                        // symbol
                                                        // is located
                                                        // between `<` and
                                                        // `>`
      lib_path / "libws_client.so", // path to the library and library name
      "plugin",                     // name of the symbol to import
      boost::dll::load_mode::append_decorations // makes
                                                //     `libmy_plugin_sum.so`
                                                // or
                                                // `my_plugin_sum.dll` from
                                                // `my_plugin_sum`
      );

  m_WsClient->init(NULL);
}
