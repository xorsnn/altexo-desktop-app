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

  m_sdk->init(NULL);
}

void Manager::onWsMessageCb(std::vector<char> msg) {
  std::cout << "AAAAAA FUCCL" << std::endl;
  std::string msgStr(msg.begin(), msg.end());
  std::cout << msgStr << std::endl;
  boost::property_tree::ptree pt;
  std::stringstream ss(msgStr);
  boost::property_tree::read_json(ss, pt);
  std::string action = pt.get<std::string>("action");
  std::cout << action << std::endl;
  if (action == "logged_in") {
    m_peerId = pt.get<std::string>("data.id");
    std::cout << m_peerId << std::endl;
    //   Q_EMIT OnSignedInSignal();
  } else if (action == "update_user_list") {
    // std::vector<CONTACT> contactList;
    contactList.clear();
    for (auto &item : pt.get_child("data")) {
      std::cout << "new item!!" << std::endl;
      std::cout << item.second.get<std::string>("name") << std::endl;
      std::cout << item.second.get<std::string>("id") << std::endl;
      CONTACT ct;
      ct.name = item.second.get<std::string>("name");
      ct.id = item.second.get<std::string>("id");
      contactList.push_back(ct);
    }

    std::cout << "yahoo!!!" << std::endl;

    // QJsonArray contactsOnline = jsonObj["data"].toArray();
    // for (int i = 0; i < contactsOnline.size(); i++) {
    //   if (contactsOnline[i].toObject()["id"].toString() != m_connId) {
    //     this->m_peers[contactsOnline[i].toObject()["id"].toString()] =
    //         contactsOnline[i].toObject()["name"].toString();
    //   }
    // }
    // Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(),
    //                              jsonObj["name"].toString());
  } else if (action == "message_from_peer") {
  }
  // } else if (action == "peer_connected") {
  //   this->m_peers[jsonObj["id"].toString()] = jsonObj["name"].toString();
  //   Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(),
  //                                jsonObj["name"].toString());
  // } else if (action == "message_from_peer") {
  //   QString tmp_msg = jsonObj["data"].toObject()["message"].toString();
  //   QJsonDocument tmp_doc = QJsonDocument::fromJson(tmp_msg.toUtf8());
  //   QJsonObject tmp_jsonObj = tmp_doc.object();
  //   if (tmp_jsonObj.contains("callAccepted")) {
  //     Q_EMIT ConnectToPeerSignal(
  //         jsonObj["data"].toObject()["sender_id"].toString());
  //   } else {
  //     Q_EMIT OnMessageFromPeerSignal(
  //         jsonObj["data"].toObject()["sender_id"].toString().toStdString(),
  //         jsonObj["data"].toObject()["message"].toString().toStdString());
  //   }
  //
  // } else if (action == "update_user_list") {
  // } else {
  //   //        Q_EMIT this->onTextMessageReceivedSignal(message);
  // }
}
