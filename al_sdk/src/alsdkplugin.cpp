#include "alsdkplugin.hpp"
#include "almanager.h"
#include <iostream>

using namespace std;

AlSdkPlugin::AlSdkPlugin() : m_debug(true) {
  if (m_debug) {
    std::cout << "AlSdkPlugin constructor" << std::endl;
  }
  m_manager = new AlManager();
  // connections
  initPeerConnectionSignal.connect(
      boost::bind(&AlManager::InitializePeerConnection, m_manager));
}

AlSdkPlugin::~AlSdkPlugin() {
  delete m_manager;
  m_manager = NULL;
}

void AlSdkPlugin::init(AlSDKCb *alSdkCb) {
  m_sdkCb = alSdkCb;
  m_manager->init(this);

  // TODO debug, checking init thread
  // m_manager->InitializePeerConnection();

  m_internalThread = boost::thread(&AlSdkPlugin::threadMain, this);
  m_manager->run();
}

void AlSdkPlugin::initializePeerConnection() {
  if (m_debug) {
    std::cout << "AlSdkPlugin::initializePeerConnection" << std::endl;
  }
  initPeerConnectionSignal();
}

void AlSdkPlugin::threadMain() {
  if (m_debug) {
    std::cout << "AlSdkPlugin::threadMain" << std::endl;
  }
  boost::this_thread::sleep(boost::posix_time::milliseconds(5 * 1000));
  initPeerConnectionSignal();
  // m_manager->run();
}

// AlWsCb *AlSdkPlugin::getWsCb() { return this; }

// void AlSdkPlugin::run() {
//   // m_manager->run();
// }
//
// bool AlSdkPlugin::hasConnections() { return m_manager->hasConnections(); }
//
// void AlSdkPlugin::InitializePeerConnection() {
//   m_manager->InitializePeerConnection();
// }
//
// void AlSdkPlugin::DeletePeerConnection() { m_manager->DeletePeerConnection();
// }
//
// void AlSdkPlugin::OnMessageFromPeer(std::string peerId,
//                                     const std::string &message) {
//   m_manager->OnMessageFromPeer(peerId, message);
// }

// void AlSdkPlugin::onMessageFromPeer(std::vector<char> peerId,
//                                     std::vector<char> message) {
//   std::string peerIdStr(peerId.begin(), peerId.end());
//   std::string msgStr(message.begin(), message.end());
//   std::cout << "===========================" << std::endl;
//   std::cout << msgStr << std::endl;
//   std::cout << "===========================" << std::endl;
//   if (m_peerId == "-1") {
//     m_peerId = peerIdStr;
//     if (m_videoDeviceName != "") {
//       //
//       m_managerInterface->setDesiredDataSource(AlManagerInterface::CAMERA);
//     } else {
//       // **
//       // * INITIATE MODE
//       // *
//       std::cout << "intiate" << std::endl;
//
//       std::vector<char> peerIdVec(m_peerId.begin(), m_peerId.end());
//
//       std::string mode = "hologram";
//       std::vector<char> modeVec(mode.begin(), mode.end());
//       m_sdkCb->initConnection(peerIdVec, modeVec);
//       // Q_EMIT requestNewFrameSignal();
//       // // TODO: take a look later
//       // //            QTimer *timer = new QTimer(this);
//       // //            connect(timer, SIGNAL(timeout()), this,
//       // //            SLOT(timeoutSlot()));
//       // //            timer->start(1000);
//     }
//   }

// boost::property_tree::ptree pt;
// std::stringstream ss(msgStr);
// boost::property_tree::read_json(ss, pt);
//
// boost::optional<std::string> sdp =
//     pt.get_optional<std::string>("data.message.sdp");
// if (sdp) {
//   std::cout << "sdb received" << std::endl;
//   //   m_managerInterface->OnMessageFromPeer(peer_id, message);
//   //   m_remoteSDP = QString::fromStdString(message);
// } else {
//   std::cout << "ice candidate received" << std::endl;
//   //   m_remoteCandidates++;
//   // qDebug() << "REMOTE CANDIDATES: ";
//   //   qDebug() << m_remoteCandidates;
//   //   curMsg[QString::fromStdString(peer_id)] =
//   //   QString::fromStdString(message);
//   //   m_messageQueue.enqueue(curMsg);
// }

// QMap<QString, QString> curMsg;
// // defining type
// QJsonDocument doc =
//     QJsonDocument::fromJson(QString::fromStdString(message).toUtf8());
// QJsonObject jsonObj = doc.object();
// qDebug() << jsonObj.contains("sdp");
//
// if (jsonObj.contains("sdp")) {
// } else {
// }
// if (!this->m_processingMsg) {
//   if (m_remoteSDP != "" && m_localSDP != "") {
//     DequeueMessagesFromPeerSlot();
//   }
// }
// }

// bool AlSdkPlugin::isClientConnected() {
//   return m_manager->m_client.is_connected();
// }
//
// void AlSdkPlugin::clientConnect(const std::string &server, int port,
//                                 const std::string &client_name) {
//   m_manager->m_client.Connect(server, port, client_name);
// }
//
// void AlSdkPlugin::clientSignOut() { m_manager->m_client.SignOut(); }
//
// void AlSdkPlugin::setDesiredDataSource(int dataSource) {
//   m_manager->setDesiredDataSource(dataSource);
// }
//
// // TODO: reconfigure this
// void AlSdkPlugin::setImageData(uint8_t *pImageBytes, size_t len, int width,
//                                int height) {
//   m_manager->setImageData(pImageBytes, len, width, height);
// }
