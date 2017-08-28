#include "manager.hpp"
// #include "altesting.hpp"
// #include "alsdkplugin.hpp"
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
      m_id(""), m_peerId("-1"), m_videoDeviceName(""),
      m_videoDeviceType(AlSdkAPI::CAMERA), m_beenCalled(false),
      m_processingCandidates(false), m_calling(false),
      m_localCandidatesCounter(0), m_remoteCandidatesCounter(0) {

  alLogger() << "Manager constructor";

  m_videoSetting.videoMode = al::MODE_2D;
  m_videoSetting.isOn = true;

  sensorList.push_back(AlTextMessage::stringToMsg("Kinect"));
  sensorList.push_back(AlTextMessage::stringToMsg("Realsense"));
}

Manager::~Manager() {

  std::cout << "Manager::~Manager()" << std::endl;

  if (m_frameThread != NULL) {
    m_frameThread->interrupt();
    m_frameThread->join();
    delete m_frameThread;
    m_frameThread = NULL;
  }
  std::cout << "Manager::~Manager()1" << std::endl;
  if (m_wsClient != NULL) {
    m_wsClient.reset();
    m_wsClient = NULL;
    m_wsClientFactory.clear();
  }
  std::cout << "Manager::~Manager()2" << std::endl;
  if (m_sensor != NULL) {
    m_sensor.reset();
    m_sensor = NULL;
  }
  std::cout << "Manager::~Manager()3" << std::endl;
  if (m_sdk != NULL) {
    m_sdk.reset();
    m_sdk = NULL;
  }
  std::cout << "Manager::~Manager()4" << std::endl;
}

void Manager::initHoloRenderer(SceneRendererCb *holoRenderer) {
  m_holoRenderer = holoRenderer;
  updateResolutionSignal.connect(
      boost::bind(&SceneRendererCb::updateResolutionCb, holoRenderer, _1, _2));
  updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::initSensor(AlSensorCb *sensorCb, SensorType sensorType) {
  boost::filesystem::path lib_path("");
  alLogger() << "Loading sensor plugin";

  switch (sensorType) {
  case KINECT_1: {
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
    m_frameThread = new boost::thread(&Manager::frameThread, this);
    // setDeviceName(sensorList[sensorList.size() - 1],
    //               AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS);
    // tread requesting new sensor frame every 30 times per second

  } break;
  case FAKE_SENSOR: {
    alLogger() << "FAKE_SENSOR";
    m_sensor = boost::dll::import<AlSensorAPI>(
        lib_path / "libal_fake_sensor.so", "plugin",
        boost::dll::load_mode::append_decorations);
    m_sensor->init(sensorCb);
    m_frameThread = new boost::thread(&Manager::frameThread, this);

  } break;
  case REALSENSE: {
    alLogger() << "REALSENSE SENSOR";
    m_sensor = boost::dll::import<AlSensorAPI>(
        lib_path / "libal_realsense.so", "plugin",
        boost::dll::load_mode::append_decorations);
    m_sensor->init(sensorCb);
    m_frameThread = new boost::thread(&Manager::frameThread, this);

  } break;
  default: { } break; }
}

void Manager::frameThread() {
  while (true) {
    m_sensor->requestNewFrame();
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000 / 30));
  }
}

/*
 * Init websocket connection
 */
void Manager::initWsConnection(AlWsCb *alWsCb) {
  boost::filesystem::path lib_path("");
  alLogger() << "Loading ws plugin";

#if defined __APPLE__
  m_wsClient = boost::dll::import<AlWsAPI>(
      lib_path / "libjson_rpc_client.dylib", "plugin",
      boost::dll::load_mode::append_decorations);
#elif defined _WIN32
  std::cout << "opening" << std::endl;
  boostfs::path pluginPath =
      boostfs::current_path() / boostfs::path("json_rpc_client_2");
  try {
    m_wsClientFactory = boostdll::import_alias<WsClientFactotry>(
        pluginPath, "create_plugin", boostdll::load_mode::append_decorations);
  } catch (const boost::system::system_error &err) {
    std::cout << "Cannot load Plugin from " << pluginPath << std::endl;
    std::cout << err.what() << std::endl;
    return;
  }
  m_wsClient = m_wsClientFactory();
  std::cout << m_wsClient << std::endl;
#else
  m_wsClient = boost::dll::import<AlWsAPI>(
      lib_path / "libjson_rpc_client_2.so", "plugin",
      boost::dll::load_mode::append_decorations);
#endif
  m_wsClient->init(alWsCb);
}

/*
 * Serving separate thread for webrtc plugin
 */
void Manager::sdkThread() {
  if (m_sdk) {
    alLogger() << "start sdk!";
    m_sdk->init(this);
    std::cout << m_sdk << std::endl;
    alLogger() << "end!";
  }
}

/*
 * Init SDK
 */
void Manager::initSdk() {
  boost::filesystem::path lib_path("");
  alLogger() << "Loading sdk plugin";

#if defined __APPLE__
  m_sdk =
      boost::dll::import<AlSdkAPI>(lib_path / "libaltexo_sdk.dylib", "plugin",
                                   boost::dll::load_mode::append_decorations);
#elif defined _WIN32
  alLogger() << "Boost DLL testing ...";

  /* Load the plugin from current working path
   * (e.g. The plugin on Windows is ${CWD}/ProgPlug.dll )
   */
  boostfs::path pluginPath = boostfs::current_path() /
                             boostfs::path("Release") /
                             boostfs::path("altexo_sdk");
  // alLogger() << "Load Plugin from " << pluginPath;

  typedef boost::shared_ptr<AlSdkAPI>(PluginCreate)();
  boost::function<PluginCreate> pluginCreator;
  try {
    pluginCreator = boostdll::import_alias<PluginCreate>(
        pluginPath, "create_plugin", boostdll::load_mode::append_decorations);
  } catch (const boost::system::system_error &err) {
    // BOOST_LOG_SEV(lg, error) << "Cannot load Plugin from " << pluginPath;
    // BOOST_LOG_SEV(lg, error) << err.what();
    return;
  }
  /* create the plugin */
  m_sdk = pluginCreator();

#else
  boost::filesystem::path lib_path2(
      "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/out/Default");

  alLogger() << "Loading the plugin";

  try {
    m_sdkPlugin = boost::dll::import<AlWebRtcPluginApi>(
        lib_path2 / "libaltexosdk.so", "plugin",
        boost::dll::load_mode::append_decorations);
  } catch (const boost::system::system_error &err) {
    // BOOST_LOG_SEV(lg, error) << "Cannot load Plugin from " << pluginPath;
    // BOOST_LOG_SEV(lg, error) << err.what();
    std::cout << err.what();
    return;
  }

  alLogger() << "Loading the plugin 0.1";
  m_sdk = boost::shared_ptr<AlSdkAPI>(m_sdkPlugin->createSdkApi());
  // m_sdk = boost::shared_ptr<AlSdkAPI>(new AlSdkPlugin());
  // AlSdkPlugin d;
  // AlTesting t;
  // t.testMethod();
  // std::cout << t.testMethod2(3, 5) << std::endl;
  alLogger() << "Loading the plugin 0.2";

#endif

  alLogger() << "Loading the plugin 1";
  m_sdkThread = new boost::thread(&Manager::sdkThread, this);

  alLogger() << "Loading the plugin 2";
  updateResolutionSignal.connect(
      boost::bind(&AlSdkAPI::updateResolution, m_sdk, _1, _2));
  updateResolutionSignal(WIDTH, HEIGHT);
}

void Manager::onIceCandidateCb(AlTextMessage msg) {
  alLogger() << "Manager::onIceCandidateCb";
  alLogger() << msg.toString();

  m_remoteCandidates.push(msg.toString());
  handleMessages();
}

void Manager::onSdpAnswerCb(AlTextMessage msg) {
  alLogger() << "Manager::onSdpAnswerCb";
  alLogger() << msg.toString();

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

// TODO: check if needed
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
  alLogger() << "Manager::onLocalSdpCb";
  alLogger() << sdp.toString();

  m_localSdp = sdp.toString();
  handleMessages();
}
void Manager::onLocalIceCandidateCb(AlTextMessage candidate) {
  alLogger() << "Manager::onLocalIceCandidateCb";
  alLogger() << candidate.toString();

  m_localCandidates.push(candidate.toString());
  handleMessages();
}

void Manager::handleMessages() {
  alLogger() << "Manager::handleMessages";
  if (!m_sentLocalSdp && m_localSdp != "") {
    if (!m_calling) {
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

      alLogger() << "local candidates: ";
      std::cout << m_localCandidatesCounter << std::endl;

      std::string candidate = m_localCandidates.front();
      m_localCandidates.pop();
      m_wsClient->sendIceCandidate(AlTextMessage(candidate));
    }
    while (!m_remoteCandidates.empty()) {
      m_remoteCandidatesCounter++;

      alLogger() << "remote candidates: ";
      std::cout << m_remoteCandidatesCounter << std::endl;

      std::string candidate = m_remoteCandidates.front();
      m_remoteCandidates.pop();
      std::vector<char> candidateVec(candidate.begin(), candidate.end());
      // TODO: migrate to AlTextMessage
      m_sdk->setRemoteIceCandidate(candidateVec);
    }
    m_processingCandidates = false;

    // NOTE: since we have connection - set desired video mode
    m_wsClient->userMode(m_videoSetting.videoMode);
  }
  if (m_sentLocalSdp && m_sentRemoteSdp && m_localCandidates.empty() &&
      m_remoteCandidates.empty() && !connectionInitialized) {
    connectionInitialized = true;
  }
}

void Manager::onNewCaptureDeciceCb(const char *newDeviceName) {
  webcamList.push_back(AlTextMessage::cStrToMsg(newDeviceName));

  // NOTE: set device name if not already set (firs is default)
  // if (m_videoDeviceName == "") {
  //   setDeviceName(AlTextMessage::cStrToMsg(newDeviceName),
  //                 AlSdkAPI::DesiredVideoSource::CAMERA);
  // }
}

void Manager::updateFrameCb(const uint8_t *image, int width, int height) {
  m_holoRenderer->updateRemoteFrameCb(image, width, height);
}

void Manager::updateLocalFrameCb(const uint8_t *image, int width, int height) {
  m_holoRenderer->updateLocalFrameCb(image, width, height);
}

void Manager::setDeviceName(alMsg deviceName,
                            AlSdkAPI::DesiredVideoSource deviceType) {
  m_videoDeviceName = AlTextMessage::msgToString(deviceName);
  m_videoDeviceType = deviceType;
  switch (m_videoDeviceType) {
  case AlSdkAPI::DesiredVideoSource::CAMERA: {
    m_holoRenderer->setLocalStreamModeCb(al::MODE_2D);
    m_videoSetting.videoMode = al::MODE_2D;
  } break;
  case AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS: {
    m_holoRenderer->setLocalStreamModeCb(al::MODE_3D);
    m_videoSetting.videoMode = al::MODE_3D;
  } break;
  default:
    break;
  }
  // TODO init it once
  _initVideoDevice();
}

void Manager::_initVideoDevice() {
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

void Manager::toggleVideo() {
  m_videoSetting.isOn = !m_videoSetting.isOn;
  if (m_videoSetting.isOn) {
    m_wsClient->userMode(m_videoSetting.videoMode);
  } else {
    m_wsClient->userMode(al::MODE_NONE);
  }
}
