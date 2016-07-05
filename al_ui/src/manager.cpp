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

  m_sensor->init(this);

  // tread requesting new sensor frame every 30 milliseconds
  boost::thread m_frameThread = boost::thread(&Manager::frameThread, this);
}

void Manager::newFrame(std::vector<uint8_t> rgbFrame) {
  // std::cout << "Manager::newFrame" << std::endl;
}

void Manager::frameThread() {
  // TODO: add finishing when destroying object
  while (true) {
    m_sensor->requestNewFrame();
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000 / 30));
  }
}

void Manager::initWsConnection() {
  boost::filesystem::path lib_path(
      "/home/xors/workspace/QT/altexo/al_ws_client/build/");
  std::cout << "Loading ws plugin" << std::endl;
  m_wsClient = boost::dll::import<AlWsClientInterface>( // type of imported
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

  m_wsClient->init(NULL);
}
