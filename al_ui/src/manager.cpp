#include "manager.hpp"
#include "allog.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>

Manager::Manager()
    : m_wsClient(NULL), m_sensor(NULL), m_sdk(NULL), m_videoDeviceType(-1),
      m_videoDeviceName(""), m_id(""), m_peerId("-1"), m_frameThread(NULL) {
  alLog("Manager constructor");
}

Manager::~Manager() {
  m_frameThread->interrupt();
  m_frameThread->join();
  delete m_frameThread;
  m_frameThread = NULL;
}

void Manager::initHoloRenderer(HologramRenderer *holoRenderer) {
  m_holoRenderer = holoRenderer;
  updateResolutionSignal.connect(
      boost::bind(&HologramRenderer::updateResolution, holoRenderer, _1, _2));
  updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::initSensor(AlSensorCb *sensorCb) {
  boost::filesystem::path lib_path("/home/xors/workspace/QT/altexo/build/");
  std::cout << "Loading sensor plugin" << std::endl;
  m_sensor = boost::dll::import<AlSensorAPI>(
      lib_path / "libal_kinect_1.so", "plugin",
      boost::dll::load_mode::append_decorations);

  // TODO move this to call initialization
  // m_sensor->init(sensorCb);
  sensorList.push_back(AlTextMessage("Kinect"));
  // set sensor as default source
  // setDeviceName(sensorList[sensorList.size() - 1],
  //               AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS);
  // tread requesting new sensor frame every 30 times per second
  m_frameThread = new boost::thread(&Manager::frameThread, this);
}

void Manager::frameThread() {
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
  updateResolutionSignal.connect(
      boost::bind(&AlSdkAPI::updateResolution, m_sdk, _1, _2));
  updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::onWsMessageCb(std::vector<char> msg) {
  std::string msgStr(msg.begin(), msg.end());
  alLog("=== ws message ===");
  alLog(msgStr);
  boost::property_tree::ptree pt;
  std::stringstream ss(msgStr);
  boost::property_tree::read_json(ss, pt);
  std::string action = pt.get<std::string>("action");
  if (action == "logged_in") {
    m_id = pt.get<std::string>("data.id");
  } else if (action == "update_user_list") {
    // updating contact list
    contactList.clear();
    for (auto &item : pt.get_child("data")) {
      CONTACT ct;
      ct.name = item.second.get<std::string>("name");
      ct.id = item.second.get<std::string>("id");
      if (ct.id != m_id) {
        contactList.push_back(ct);
      }
    }
  } else if (action == "message_from_peer") {
    onMessageFromPeer(pt);
  }
}

void Manager::initConnection(std::string peerId) {
  // TODO: controll assigning once for a call
  m_peerId = peerId;
  if (m_wsClient != NULL) {
    std::ostringstream stream;
    boost::property_tree::ptree pt;
    pt.put("call", true);
    boost::property_tree::write_json(stream, pt, false);
    std::string strJson = stream.str();
    m_wsClient->sendMessageToPeer(AlTextMessage(m_peerId),
                                  AlTextMessage(strJson));
  }
}

void Manager::setConnectionMode(std::string mode) {
  if (m_wsClient != NULL) {
    std::ostringstream stream;
    boost::property_tree::ptree pt;
    pt.put("mode", mode);
    boost::property_tree::write_json(stream, pt, false);
    std::string strJson = stream.str();
    m_wsClient->sendMessageToPeer(AlTextMessage(m_peerId),
                                  AlTextMessage(strJson));
  }
}

void Manager::onMessageFromPeer(boost::property_tree::ptree msgPt) {
  std::string senderIdStr = msgPt.get<std::string>("data.sender_id");
  std::string messageStr = msgPt.get<std::string>("data.message");

  boost::property_tree::ptree jsonMsg;
  std::stringstream ss(messageStr);
  boost::property_tree::read_json(ss, jsonMsg);

  alLog(messageStr);

  boost::optional<bool> callAccepted =
      jsonMsg.get_optional<bool>("callAccepted");

  boost::optional<bool> isCall = jsonMsg.get_optional<bool>("call");

  boost::optional<std::string> mode = jsonMsg.get_optional<std::string>("mode");

  // this is the very first contact we will store peer id
  if (m_peerId == "-1") {
    m_peerId = senderIdStr;
    _initVideoDevice();
  }

  if (callAccepted) {
    alLog("callAccepted");
    _initVideoDevice();
    m_sdk->initializePeerConnection();
  } else if (isCall) {
    alLog("isCall");
    // TODO implement accept call functionality
    std::ostringstream stream;
    boost::property_tree::ptree msgToSendPt;
    msgToSendPt.put("callAccepted", true);
    boost::property_tree::write_json(stream, msgToSendPt, false);
    std::string strJson = stream.str();
    m_wsClient->sendMessageToPeer(AlTextMessage(m_peerId),
                                  AlTextMessage(strJson));
  } else if (mode) {
    // TODO: handle this
  } else {
    alLog("================");
    alLog("sdp or candidate");
    alLog("================");
    boost::optional<std::string> sdp = jsonMsg.get_optional<std::string>("sdp");
    if (sdp) {
      m_remoteSdp = messageStr;
    } else {
      m_remoteCandidates.push(messageStr);
    }
    handleMessages();
  }
}

void Manager::onSdp(std::vector<char> sdp) {
  alLog("Manager::onSdp");
  m_localSdp = std::string(sdp.begin(), sdp.end());
  handleMessages();
}
void Manager::onCandidate(std::vector<char> candidate) {
  alLog("Manager::onCandidate");
  m_localCandidates.push(std::string(candidate.begin(), candidate.end()));
  handleMessages();
}

void Manager::handleMessages() {
  if (!m_sentLocalSdp && m_localSdp != "") {
    m_wsClient->sendMessageToPeer(AlTextMessage(m_peerId),
                                  AlTextMessage(m_localSdp));
    m_sentLocalSdp = true;
  }
  if (!m_sentRemoteSdp && m_remoteSdp != "") {
    std::vector<char> remoteSdpVec(m_remoteSdp.begin(), m_remoteSdp.end());
    m_sdk->setRemoteSdp(remoteSdpVec);
    m_sentRemoteSdp = true;
  }
  if (m_sentLocalSdp && m_sentRemoteSdp) {
    while (!m_localCandidates.empty()) {
      std::string candidate = m_localCandidates.front();
      m_localCandidates.pop();
      m_wsClient->sendMessageToPeer(AlTextMessage(m_peerId),
                                    AlTextMessage(candidate));
    }
    while (!m_remoteCandidates.empty()) {
      std::string candidate = m_remoteCandidates.front();
      m_remoteCandidates.pop();
      std::vector<char> candidateVec(candidate.begin(), candidate.end());
      m_sdk->setRemoteIceCandidate(candidateVec);
    }
  }
  if (m_sentLocalSdp && m_sentRemoteSdp && m_localCandidates.empty() &&
      m_remoteCandidates.empty() && !connectionInitialized) {
    connectionInitialized = true;
  }
}

void Manager::onDevicesListChangedCb(std::vector<AlTextMessage> deviceNames) {
  alLog("Manager::onDevicesListChangedCb");
  webcamList = deviceNames;
}

void Manager::updateFrameCb(const uint8_t *image, int width, int height) {
  m_holoRenderer->updateRemoteFrame(image, width, height);
}

void Manager::updateLocalFrameCb(const uint8_t *image, int width, int height) {
  m_holoRenderer->updateLocalFrame(image, width, height);
}

void Manager::setDeviceName(AlTextMessage deviceName, int deviceType) {
  m_videoDeviceName = deviceName.getText();
  m_videoDeviceType = deviceType;
}

void Manager::callToPeer(std::string peerId) { initConnection(peerId); }

void Manager::_initVideoDevice() {
  switch (m_videoDeviceType) {
  case AlSdkAPI::DesiredVideoSource::CAMERA: {
    m_sdk->setDesiredDataSource(m_videoDeviceType);
    m_sdk->setDesiredVideDeviceName(m_videoDeviceName);
    setConnectionMode("audio+video");
  } break;
  case AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS: {
    m_sdk->setDesiredDataSource(m_videoDeviceType);
    // setConnectionMode("audio+video");
    setConnectionMode("hologram");
  } break;
  default: {
    // unhandled video device
  }
  }
}
