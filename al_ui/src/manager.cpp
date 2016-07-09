#include "manager.hpp"
#include <iostream>
Manager::Manager() : m_wsClient(NULL), m_sensor(NULL), m_sdk(NULL) {
  std::cout << "Manager constructor" << std::endl;
}

Manager::~Manager() {}

void Manager::initSensor(AlSensorCb *sensorCb) {
  boost::filesystem::path lib_path("/home/xors/workspace/QT/altexo/build/");
  std::cout << "Loading sensor plugin" << std::endl;
  m_sensor = boost::dll::import<AlSensorAPI>(
      lib_path / "libal_kinect_1.so", "plugin",
      boost::dll::load_mode::append_decorations);

  m_sensor->init(sensorCb);

  // tread requesting new sensor frame every 30 milliseconds
  boost::thread m_frameThread = boost::thread(&Manager::frameThread, this);
}

void Manager::frameThread() {
  // TODO: add finishing when destroying object
  while (true) {
    m_sensor->requestNewFrame();
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000 / 30));
  }
}

void Manager::initWsConnection(AlWsCb *alWsCb) {
  boost::filesystem::path lib_path("/home/xors/workspace/QT/altexo/build/");
  std::cout << "Loading ws plugin" << std::endl;
  m_wsClient =
      boost::dll::import<AlWsAPI>(lib_path / "libws_client.so", "plugin",
                                  boost::dll::load_mode::append_decorations);

  m_wsClient->init(alWsCb);
}

void Manager::initSdk() {
  boost::filesystem::path lib_path("/home/xors/workspace/QT/altexo/build/");
  std::cout << "Loading sdk plugin" << std::endl;
  m_sdk =
      boost::dll::import<AlSdkAPI>(lib_path / "libaltexo_sdk.so", "plugin",
                                   boost::dll::load_mode::append_decorations);

  m_sdk->init(NULL);
}
