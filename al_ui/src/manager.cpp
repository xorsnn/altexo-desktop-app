#include "manager.hpp"
#include "contact.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
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

  m_sdk->init(this);
  newMessageToSdk.connect(
      boost::bind(&AlSdkAPI::onMessageFromPeer, m_sdk, _1, _2));
}

void Manager::onWsMessageCb(std::vector<char> msg) {
  // std::cout << "AAAAAA FUCCL" << std::endl;
  std::string msgStr(msg.begin(), msg.end());
  // std::cout << msgStr << std::endl;
  boost::property_tree::ptree pt;
  std::stringstream ss(msgStr);
  boost::property_tree::read_json(ss, pt);
  std::string action = pt.get<std::string>("action");
  // std::cout << action << std::endl;
  if (action == "logged_in") {
    m_id = pt.get<std::string>("data.id");
  } else if (action == "update_user_list") {
    // updating contact list
    contactList.clear();
    for (auto &item : pt.get_child("data")) {
      CONTACT ct;
      ct.name = item.second.get<std::string>("name");
      ct.id = item.second.get<std::string>("id");
      contactList.push_back(ct);
    }
  } else if (action == "message_from_peer") {
    std::string messageStr = pt.get<std::string>("data.message");
    std::string senderIdStr = pt.get<std::string>("data.sender_id");
    std::cout << messageStr << std::endl;
    boost::optional<bool> callAccepted =
        pt.get_optional<bool>("data.message.callAccepted");
    if (callAccepted) {
      std::cout << "callAccepted" << std::endl;
      //     Q_EMIT ConnectToPeerSignal(
      //         jsonObj["data"].toObject()["sender_id"].toString());
    } else {
      std::cout << "not callAccepted" << std::endl;
      std::vector<char> message(messageStr.begin(), messageStr.end());
      std::vector<char> senderId(senderIdStr.begin(), senderIdStr.end());
      newMessageToSdk(senderId, message);
      //     Q_EMIT OnMessageFromPeerSignal(
      //         jsonObj["data"].toObject()["sender_id"].toString().toStdString(),
      //         jsonObj["data"].toObject()["message"].toString().toStdString());
    }
  }
}

void Manager::initConnection(std::vector<char> peerId, std::vector<char> mode) {
  std::ostringstream stream;
  boost::property_tree::ptree pt;
  pt.put("mode", "hologram");
  boost::property_tree::write_json(stream, pt);
  std::string strJson = stream.str();
  std::cout << "<<<<<" << std::endl;
  std::cout << strJson << std::endl;
}
