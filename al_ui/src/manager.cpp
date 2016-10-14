#include "manager.hpp"
#include "allog.hpp"
#include "boost/dll/import.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include <boost/dll/import.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>

namespace boostfs = boost::filesystem;
namespace boostdll = boost::dll;

// TODO: repeated over multiple files, move to a separate lib
template <typename T>
std::vector<T> as_vector(boost::property_tree::ptree const &pt,
                         boost::property_tree::ptree::key_type const &key) {
  std::vector<T> r;
  for (auto &item : pt.get_child(key))
    r.push_back(item.second.get_value<T>());
  return r;
}

Manager::Manager()
    : m_sdk(NULL), m_wsClient(NULL), m_sensor(NULL), m_frameThread(NULL),
      m_id(""), m_peerId("-1"), m_videoDeviceName(""), m_videoDeviceType(-1),
      m_beenCalled(false), m_processingCandidates(false), m_calling(false),
      m_localCandidatesCounter(0), m_remoteCandidatesCounter(0) {
  alLog("Manager constructor");
  sensorList.push_back(AlTextMessage("Kinect"));
}

Manager::~Manager() {
  if (m_frameThread != NULL) {
    m_frameThread->interrupt();
    m_frameThread->join();
    delete m_frameThread;
    m_frameThread = NULL;
  }
  if (m_wsClient != NULL) {
    m_wsClient.reset();
    m_wsClient = NULL;
  }
  if (m_sensor != NULL) {
    m_sensor.reset();
    m_sensor = NULL;
  }
  if (m_sdk != NULL) {
    m_sdk.reset();
    m_sdk = NULL;
  }
}

void Manager::initHoloRenderer(SceneRenderer *holoRenderer) {
  m_holoRenderer = holoRenderer;
  updateResolutionSignal.connect(
      boost::bind(&SceneRenderer::updateResolution, holoRenderer, _1, _2));
  updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::initSensor(AlSensorCb *sensorCb) {
  boost::filesystem::path lib_path("");
  std::cout << "Loading sensor plugin" << std::endl;
#ifdef __APPLE__
  m_sensor = boost::dll::import<AlSensorAPI>(
      lib_path / "libal_kinect_1.dylib", "plugin",
      boost::dll::load_mode::append_decorations);
#else
  m_sensor = boost::dll::import<AlSensorAPI>(
      lib_path / "libal_kinect_1.so", "plugin",
      boost::dll::load_mode::append_decorations);
#endif

  // TODO move this to call initialization
  m_sensor->init(sensorCb);
  // sensorList.push_back(AlTextMessage("Kinect"));
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
  boost::filesystem::path lib_path("");
  std::cout << "Loading ws plugin" << std::endl;
// TODO: remove when migrate to json rpc
// #ifdef __APPLE__
//   m_wsClient =
//       boost::dll::import<AlWsAPI>(lib_path / "libws_client.dylib", "plugin",
//                                   boost::dll::load_mode::append_decorations);
// #else
//   m_wsClient =
//       boost::dll::import<AlWsAPI>(lib_path / "libws_client.so", "plugin",
//                                   boost::dll::load_mode::append_decorations);
// #endif
#ifdef __APPLE__
  m_wsClient = boost::dll::import<AlWsAPI>(
      lib_path / "libjson_rpc_client.dylib", "plugin",
      boost::dll::load_mode::append_decorations);
#else
  m_wsClient =
      boost::dll::import<AlWsAPI>(lib_path / "libjson_rpc_client.so", "plugin",
                                  boost::dll::load_mode::append_decorations);
#endif
  m_wsClient->init(alWsCb);

  // TEST
  // m_wsClient->sendMessage(AlTextMessage("{123}"));
}

void Manager::initSdk() {
  boost::filesystem::path lib_path("");
  std::cout << "Loading sdk plugin" << std::endl;
#ifdef __APPLE__
  m_sdk =
      boost::dll::import<AlSdkAPI>(lib_path / "libaltexo_sdk.dylib", "plugin",
                                   boost::dll::load_mode::append_decorations);
#else
#ifdef _WIN32
  std::cout << "Boost DLL testing ..." << std::endl;

  /* Load the plugin from current working path
   * (e.g. The plugin on Windows is ${CWD}/ProgPlug.dll )
   */
  boostfs::path pluginPath = boostfs::current_path() /
                             boostfs::path("Release") /
                             boostfs::path("altexo_sdk");
  std::cout << "Load Plugin from " << pluginPath << std::endl;

  typedef boost::shared_ptr<AlSdkAPI>(PluginCreate)();
  boost::function<PluginCreate> pluginCreator;
  try {
    pluginCreator = boostdll::import_alias<PluginCreate>(
        pluginPath, "create_plugin", boostdll::load_mode::append_decorations);
  } catch (const boost::system::system_error &err) {
    std::cerr << "Cannot load Plugin from " << pluginPath << std::endl;
    std::cerr << err.what() << std::endl;
    return;
  }
  /* create the plugin */
  // auto plugin = pluginCreator();
  m_sdk = pluginCreator();
#else
  m_sdk =
      boost::dll::import<AlSdkAPI>(lib_path / "libaltexo_sdk.so", "plugin",
                                   boost::dll::load_mode::append_decorations);
#endif
#endif
  m_sdk->init(this);
  updateResolutionSignal.connect(
      boost::bind(&AlSdkAPI::updateResolution, m_sdk, _1, _2));
  updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::onIceCandidateCb(AlTextMessage msg) {
  std::cout << "Manager::onIceCandidateCb" << std::endl;
  std::cout << msg.toString() << std::endl;

  m_remoteCandidates.push(msg.toString());
  handleMessages();
}

void Manager::onSdpAnswerCb(AlTextMessage msg) {
  std::cout << "Manager::onSdpAnswerCb" << std::endl;
  std::cout << msg.toString() << std::endl;

  boost::property_tree::ptree pt;
  std::stringstream ss(msg.toString());
  boost::property_tree::read_json(ss, pt);
  std::string sdpBody = pt.get<std::string>("result");
  if (sdpBody == "") {
    for (auto i : as_vector<std::string>(pt, "result")) {
      sdpBody = i;
    }
  }

  std::ostringstream streamRes;
  boost::property_tree::ptree ptRes;
  ptRes.put("sdp", sdpBody);
  ptRes.put("type", "answer");
  boost::property_tree::write_json(streamRes, ptRes, false);

  m_remoteSdp = streamRes.str();

  handleMessages();
}

void Manager::onSdpOfferCb(AlTextMessage msg) {
  std::cout << "Manager::onSdpOfferCb" << std::endl;
  std::cout << msg.toString() << std::endl;
  // **
  // TODO: this is temprorary solution, we make messages similar to what we used
  // before
  boost::property_tree::ptree pt;
  std::stringstream ss(msg.toString());
  boost::property_tree::read_json(ss, pt);
  std::string sdpBody;
  for (auto i : as_vector<std::string>(pt, "params")) {
    sdpBody = i;
  }
  std::ostringstream streamRes;
  boost::property_tree::ptree ptRes;
  ptRes.put("sdp", sdpBody);
  ptRes.put("type", "offer");
  boost::property_tree::write_json(streamRes, ptRes, false);

  m_remoteSdp = streamRes.str();

  handleMessages();
}

void Manager::onInitCall() {
  m_calling = true;
  _initVideoDevice();
  m_sdk->initializePeerConnection();
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

// TODO: reimplement for new rpc
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

void Manager::onLocalSdpCb(AlTextMessage sdp) {
  alLog("Manager::onLocalSdpCb");
  std::cout << sdp.toString() << std::endl;
  m_localSdp = sdp.toString();
  handleMessages();
}
void Manager::onLocalIceCandidateCb(AlTextMessage candidate) {
  alLog("Manager::onLocalIceCandidateCb");
  // std::cout << candidate.toString() << std::endl;
  m_localCandidates.push(candidate.toString());
  handleMessages();
}

void Manager::handleMessages() {
  if (!m_sentLocalSdp && m_localSdp != "") {
    if (!m_calling) {
      m_wsClient->sendSdpAnswer(AlTextMessage(m_localSdp));
    } else {
      m_wsClient->sendSdpOffer(AlTextMessage(m_localSdp));
    }
    m_sentLocalSdp = true;
  }
  if (!m_sentRemoteSdp && m_remoteSdp != "") {
    std::vector<char> remoteSdpVec(m_remoteSdp.begin(), m_remoteSdp.end());
    m_sdk->setRemoteSdp(remoteSdpVec);
    m_sentRemoteSdp = true;
  }
  if (m_sentLocalSdp && m_sentRemoteSdp && !m_processingCandidates) {
    m_processingCandidates = true;
    while (!m_localCandidates.empty()) {
      m_localCandidatesCounter++;
      std::cout << "local candidates: " << std::endl;
      std::cout << m_localCandidatesCounter << std::endl;

      std::string candidate = m_localCandidates.front();
      m_localCandidates.pop();
      std::cout << candidate << std::endl;
      m_wsClient->sendIceCandidate(AlTextMessage(candidate));
    }
    while (!m_remoteCandidates.empty()) {
      m_remoteCandidatesCounter++;
      std::cout << "remote candidates: " << std::endl;
      std::cout << m_remoteCandidatesCounter << std::endl;

      std::string candidate = m_remoteCandidates.front();
      m_remoteCandidates.pop();
      std::vector<char> candidateVec(candidate.begin(), candidate.end());
      // TODO: migrate to AlTextMessage
      m_sdk->setRemoteIceCandidate(candidateVec);
    }
    m_processingCandidates = false;
  }
  if (m_sentLocalSdp && m_sentRemoteSdp && m_localCandidates.empty() &&
      m_remoteCandidates.empty() && !connectionInitialized) {
    connectionInitialized = true;
  }
}

void Manager::onDevicesListChangedCb(std::vector<AlTextMessage> deviceNames) {
  webcamList = deviceNames;
  // NOTE: setting default cam
  // TODO: remember last choice
  setDeviceName(webcamList[0], AlSdkAPI::DesiredVideoSource::CAMERA);
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
  switch (m_videoDeviceType) {
  case AlSdkAPI::DesiredVideoSource::CAMERA: {
    m_holoRenderer->setLocalStreamMode(SceneRenderer::AUDIO_VIDEO);
  } break;
  case AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS: {
    m_holoRenderer->setLocalStreamMode(SceneRenderer::HOLOGRAM);
  } break;
  default:
    break;
  }
  // TODO init it once
  _initVideoDevice();
}

void Manager::_initVideoDevice() {
  std::cout << "Manager::_initVideoDevice" << std::endl;
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
