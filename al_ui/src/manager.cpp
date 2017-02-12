#include "manager.hpp"
#include "AL_API/alwebrtcpluginapi.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include <boost/dll/import.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>

namespace boostfs = boost::filesystem;
namespace boostdll = boost::dll;

using namespace boost::log::trivial;
// boost::log::sources::severity_logger<severity_level> lg;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
    al_logger, boost::log::sources::severity_logger<severity_level>);
boost::log::sources::severity_logger<severity_level> &lg = al_logger::get();

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
  BOOST_LOG_SEV(lg, debug) << "Manager constructor";
  sensorList.push_back(AlTextMessage::stringToMsg("Kinect"));
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
  BOOST_LOG_SEV(lg, debug) << "Loading sensor plugin";

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
  BOOST_LOG_SEV(lg, debug) << "Loading ws plugin";

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
  alLogger() << "Loading sdk plugin";

#ifdef __APPLE__
  m_sdk =
      boost::dll::import<AlSdkAPI>(lib_path / "libaltexo_sdk.dylib", "plugin",
                                   boost::dll::load_mode::append_decorations);
#else
#ifdef _WIN32
  BOOST_LOG_SEV(lg, debug) << "Boost DLL testing ...";

  /* Load the plugin from current working path
   * (e.g. The plugin on Windows is ${CWD}/ProgPlug.dll )
   */
  boostfs::path pluginPath = boostfs::current_path() /
                             boostfs::path("Release") /
                             boostfs::path("altexo_sdk");
  BOOST_LOG_SEV(lg, debug) << "Load Plugin from " << pluginPath;

  typedef boost::shared_ptr<AlSdkAPI>(PluginCreate)();
  boost::function<PluginCreate> pluginCreator;
  try {
    pluginCreator = boostdll::import_alias<PluginCreate>(
        pluginPath, "create_plugin", boostdll::load_mode::append_decorations);
  } catch (const boost::system::system_error &err) {
    BOOST_LOG_SEV(lg, error) << "Cannot load Plugin from " << pluginPath;
    BOOST_LOG_SEV(lg, error) << err.what();
    return;
  }
  /* create the plugin */
  m_sdk = pluginCreator();
#else
  // m_sdk =
  //     boost::dll::import<AlSdkAPI>(lib_path / "libaltexo_sdk_2_0.so",
  //     "plugin",
  //                                  boost::dll::load_mode::append_decorations);
  boost::filesystem::path lib_path2(
      "/home/xors/workspace/lib/webrtc-checkout/src/out/Default");

  boost::shared_ptr<AlWebRtcPluginApi> plugin;
  alLogger() << "Loading the plugin";

  plugin = boost::dll::import<AlWebRtcPluginApi>(
      lib_path2 / "libpeerconnection_2.so", "plugin",
      boost::dll::load_mode::append_decorations);
  m_sdk = boost::shared_ptr<AlSdkAPI>(plugin->createSdkApi());
#endif
#endif
  m_sdk->init(this);
  // updateResolutionSignal.connect(
  //     boost::bind(&AlSdkAPI::updateResolution, m_sdk, _1, _2));
  // updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::onIceCandidateCb(AlTextMessage msg) {
  BOOST_LOG_SEV(lg, debug) << "Manager::onIceCandidateCb";
  BOOST_LOG_SEV(lg, debug) << msg.toString();

  m_remoteCandidates.push(msg.toString());
  handleMessages();
}

void Manager::onSdpAnswerCb(AlTextMessage msg) {
  BOOST_LOG_SEV(lg, debug) << "Manager::onSdpAnswerCb";
  BOOST_LOG_SEV(lg, debug) << msg.toString();

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

void Manager::onSdpOfferCb(const char *cMsg) {
  alLogger() << "Manager::onSdpOfferCb";
  AlTextMessage msg = AlTextMessage::cStrToMsg(cMsg);
  // BOOST_LOG_SEV(lg, debug) << "Manager::onSdpOfferCb";
  // BOOST_LOG_SEV(lg, debug) << msg.toString();

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

  alLogger() << "> Manager::onSdpOfferCb end!";
  handleMessages();
  alLogger() << "> Manager::onSdpOfferCb end! 2";
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
  BOOST_LOG_SEV(lg, debug) << "Manager::onLocalSdpCb";
  BOOST_LOG_SEV(lg, debug) << sdp.toString();

  m_localSdp = sdp.toString();
  handleMessages();
}
void Manager::onLocalIceCandidateCb(AlTextMessage candidate) {
  BOOST_LOG_SEV(lg, debug) << "Manager::onLocalIceCandidateCb";

  m_localCandidates.push(candidate.toString());
  handleMessages();
}

void Manager::handleMessages() {
  alLogger() << "Manager::handleMessages";
  if (!m_sentLocalSdp && m_localSdp != "") {
    if (!m_calling) {
      alLogger() << "> 2";
      m_wsClient->sendSdpAnswer(AlTextMessage(m_localSdp));
    } else {
      char *cstr = new char[m_localSdp.length() + 1];
      cstr[m_localSdp.length()] = '\n';
      std::strcpy(cstr, m_localSdp.c_str());
      m_wsClient->sendSdpOffer(cstr);
      delete[] cstr;
    }
    m_sentLocalSdp = true;
  }
  if (!m_sentRemoteSdp && m_remoteSdp != "") {
    alLogger() << "> 3";
    // std::vector<char> remoteSdpVec(m_remoteSdp.begin(), m_remoteSdp.end());
    char *cstr = new char[m_remoteSdp.length() + 1];
    cstr[m_remoteSdp.length()] = '\n';
    std::strcpy(cstr, m_remoteSdp.c_str());
    m_sdk->setRemoteSdp(cstr);
    delete[] cstr;
    m_sentRemoteSdp = true;
  }
  if (m_sentLocalSdp && m_sentRemoteSdp && !m_processingCandidates) {
    m_processingCandidates = true;
    while (!m_localCandidates.empty()) {
      m_localCandidatesCounter++;
      BOOST_LOG_SEV(lg, debug) << "local candidates: "
                               << m_localCandidatesCounter;

      std::string candidate = m_localCandidates.front();
      m_localCandidates.pop();
      m_wsClient->sendIceCandidate(AlTextMessage(candidate));
    }
    while (!m_remoteCandidates.empty()) {
      m_remoteCandidatesCounter++;
      BOOST_LOG_SEV(lg, debug) << "remote candidates: "
                               << m_remoteCandidatesCounter;

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

void Manager::onNewCaptureDeciceCb(const char *newDeviceName) {
  webcamList.push_back(AlTextMessage::cStrToMsg(newDeviceName));

  // NOTE: set device name if not already set (firs is default)
  if (m_videoDeviceName == "") {
    setDeviceName(AlTextMessage::cStrToMsg(newDeviceName),
                  AlSdkAPI::DesiredVideoSource::CAMERA);
  }
}

void Manager::updateFrameCb(const uint8_t *image, int width, int height) {
  m_holoRenderer->updateRemoteFrame(image, width, height);
}

void Manager::updateLocalFrameCb(const uint8_t *image, int width, int height) {
  m_holoRenderer->updateLocalFrame(image, width, height);
}

void Manager::setDeviceName(alMsg deviceName, int deviceType) {
  alLogger() << "Manager::setDeviceName";
  alLogger() << "*************************";
  m_videoDeviceName = AlTextMessage::msgToString(deviceName);
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
  alLogger() << "Manager::_initVideoDevice";

  switch (m_videoDeviceType) {
  case AlSdkAPI::DesiredVideoSource::CAMERA: {
    m_sdk->setDesiredDataSource(m_videoDeviceType);

    char *cstr = new char[m_videoDeviceName.length() + 1];
    cstr[m_videoDeviceName.length()] = '\n';
    std::strcpy(cstr, m_videoDeviceName.c_str());
    m_sdk->setDesiredVideDeviceName(cstr);
    delete[] cstr;

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
