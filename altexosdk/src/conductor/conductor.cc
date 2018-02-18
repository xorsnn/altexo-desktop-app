#include "altexosdk/src/conductor/conductor.h"

#include <iostream>
#include <utility>
#include <vector>

#include "AL_API/sdk_api.hpp"
#include "altexosdk/src/conductor/defaults.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/test/fakeconstraints.h"
#include "media/engine/webrtcvideocapturerfactory.h"
#include "modules/video_capture/video_capture_factory.h"
#include "rtc_base/checks.h"
#include "rtc_base/json.h"
#include "rtc_base/logging.h"

using webrtc::SdpType;

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

#define DTLS_ON true
#define DTLS_OFF false

class DummySetSessionDescriptionObserver
    : public webrtc::SetSessionDescriptionObserver {
public:
  static DummySetSessionDescriptionObserver *Create() {
    return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
  }
  virtual void OnSuccess() {}

  virtual void OnFailure(const std::string &error) {}

protected:
  DummySetSessionDescriptionObserver() {}
  ~DummySetSessionDescriptionObserver() {}
};

Conductor::Conductor(AlCallback *alCallback)
    : m_alCallback(alCallback), m_debug(true), m_processingMsg(false),
      m_isAcceptingConnection(false) {
  /*
   * TODO: temporary solution, refactor later
   */
  m_dataManager = new AlDataManager();

  // notify about avaliable cameras
  std::vector<std::string> deviceNames;
  {
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    if (info) {
      int num_devices = info->NumberOfDevices();
      for (int i = 0; i < num_devices; ++i) {
        const uint32_t kSize = 256;
        char name[kSize] = {0};
        char id[kSize] = {0};
        if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
          deviceNames.push_back(name);
        }
      }
    }
  }
  std::vector<std::vector<char>> deviceNamesMsg;
  for (unsigned long i = 0; i < deviceNames.size(); i++) {
    deviceNamesMsg.push_back(AlTextMessage::stringToMsg(deviceNames[i]));
  }
  m_alCallback->onDevicesListChangedCb(deviceNamesMsg);
}

Conductor::~Conductor() {}

bool Conductor::connection_active() const {
  return m_peerConnection.get() != NULL;
}

void Conductor::Close() {}

bool Conductor::InitializePeerConnection() {
  RTC_DCHECK(peer_connection_factory_.get() == NULL);
  RTC_DCHECK(m_peerConnection.get() == NULL);

  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
      webrtc::CreateBuiltinAudioEncoderFactory(),
      webrtc::CreateBuiltinAudioDecoderFactory());

  if (!peer_connection_factory_.get()) {
    DeletePeerConnection();
    return false;
  }

  if (!CreatePeerConnection(DTLS_ON)) {
    DeletePeerConnection();
  }
  AddStreams();

  return m_peerConnection.get() != NULL;
}

bool Conductor::CreatePeerConnection(bool dtls) {
  RTC_DCHECK(peer_connection_factory_.get() != NULL);
  RTC_DCHECK(m_peerConnection.get() == NULL);

  webrtc::PeerConnectionInterface::RTCConfiguration config;
  webrtc::PeerConnectionInterface::IceServer server;
  server.uri = GetPeerConnectionString();
  config.servers.push_back(server);

  webrtc::FakeConstraints constraints;
  if (dtls) {
    constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
                            "true");
  } else {
    constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
                            "false");
  }

  m_peerConnection = peer_connection_factory_->CreatePeerConnection(
      config, &constraints, NULL, NULL, this);

  return m_peerConnection.get() != NULL;
}

void Conductor::DeletePeerConnection() {}

// TODO: remove method
void Conductor::EnsureStreamingUI() {}

// **
// * PeerConnectionObserver implementation.
// *

// Called when a remote stream is added
void Conductor::OnAddStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {

  stream->AddRef();
  webrtc::VideoTrackVector tracks = stream->GetVideoTracks();

  // Only render the first track.
  if (!tracks.empty()) {
    webrtc::VideoTrackInterface *track = tracks[0];

    // TODO additional method
    // Video track

    m_remoteRenderer.reset(
        new AlVideoRenderer(1, track, m_alCallback, AlVideoRenderer::REMOTE));
  }
  stream->Release();
}

void Conductor::OnRemoveStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {
  stream.release();
}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {
  if (m_debug) {
  }

  Json::StyledWriter writer;
  Json::Value jmessage;

  jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
  jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
  std::string sdp;
  if (!candidate->ToString(&sdp)) {
    return;
  }
  jmessage[kCandidateSdpName] = sdp;
  // TODO move to callback
  std::string *msg = new std::string(writer.write(jmessage));
  queueUIThreadCallback(SEND_LOCAL_CANDIDATE, msg);
}

////
//// PeerConnectionClientObserver implementation.
////

void Conductor::OnMessageFromPeer(std::string peer_id,
                                  std::vector<char> msgVec) {
  std::string message(msgVec.begin(), msgVec.end());

  // RTC_DCHECK(peer_id_ == peer_id || peer_id_ == "-1");
  RTC_DCHECK(!message.empty());

  if (!m_peerConnection.get()) {
    // TODO HACK!
    this->m_isAcceptingConnection = true;

    if (!InitializePeerConnection()) {
      return;
    }
  }

  Json::Reader reader;
  Json::Value jmessage;
  if (!reader.parse(message, jmessage)) {
    return;
  }
  std::string type;
  std::string json_object;

  rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName, &type);
  if (!type.empty()) {
    std::string sdp;
    if (!rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName,
                                      &sdp)) {

      return;
    }
    webrtc::SdpParseError error;
    webrtc::SessionDescriptionInterface *session_description(
        webrtc::CreateSessionDescription(type, sdp, &error));
    if (!session_description) {
      return;
    }
    m_peerConnection->SetRemoteDescription(
        DummySetSessionDescriptionObserver::Create(), session_description);
    if (session_description->type() ==
        webrtc::SessionDescriptionInterface::kOffer) {
      m_peerConnection->CreateAnswer(this, NULL);
    }
    return;
  } else {
    std::string sdp_mid;
    int sdp_mlineindex = 0;
    std::string sdp;

    if (!rtc::GetStringFromJsonObject(jmessage, kCandidateSdpMidName,
                                      &sdp_mid) ||
        !rtc::GetIntFromJsonObject(jmessage, kCandidateSdpMlineIndexName,
                                   &sdp_mlineindex) ||
        !rtc::GetStringFromJsonObject(jmessage, kCandidateSdpName, &sdp)) {
      return;
    }
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::IceCandidateInterface> candidate(
        webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp, &error));
    if (!candidate.get()) {
      return;
    }
    if (!m_peerConnection->AddIceCandidate(candidate.get())) {
      return;
    } else {
    }
    return;
  }
}

std::unique_ptr<cricket::VideoCapturer> Conductor::OpenVideoCaptureDevice() {
  if (m_debug) {
  }

  if (m_dataManager->m_desiredVideoSource ==
      AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS) {
    /*
     * TODO: temporary solution, should never be reached
     */
    //  sending image snapshots
    if (m_debug) {
    }

    AlVideoCapturer *capturer = new AlVideoCapturer();

    m_dataManager->startVideoCapturer(capturer);

    return std::unique_ptr<cricket::VideoCapturer>(capturer);
  } else {
    std::vector<std::string> device_names;
    {
      std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
          webrtc::VideoCaptureFactory::CreateDeviceInfo());
      if (!info) {
        return nullptr;
      }
      int num_devices = info->NumberOfDevices();
      for (int i = 0; i < num_devices; ++i) {
        const uint32_t kSize = 256;
        char name[kSize] = {0};
        char id[kSize] = {0};
        if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
          device_names.push_back(name);
        }
      }
    }

    cricket::WebRtcVideoDeviceCapturerFactory factory;
    std::unique_ptr<cricket::VideoCapturer> capturer = nullptr;

    for (const auto &name : device_names) {
      if (m_alCallback->getVideoDeviceName() == "") {
        capturer = factory.Create(cricket::Device(name, 0));
        if (capturer) {
          break;
        }
      } else {
        if (m_alCallback->getVideoDeviceName() == name) {
          capturer = factory.Create(cricket::Device(name, 0));
          if (capturer) {
            break;
          }
        }
      }
    }
    return capturer;
  }
}

void Conductor::AddStreams() {
  if (m_debug) {
  }
  if (m_activeStreams.find(kStreamLabel) != m_activeStreams.end()) {
    return; // Already added.
  }

  rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
      peer_connection_factory_->CreateAudioTrack(
          kAudioLabel, peer_connection_factory_->CreateAudioSource(NULL)));

  rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
      peer_connection_factory_->CreateVideoTrack(
          kVideoLabel, peer_connection_factory_->CreateVideoSource(
                           OpenVideoCaptureDevice(), NULL)));

  // Starting local renderer
  /*
   * TODO: fix, split local and remote renderer
   */
  m_localRenderer.reset(new AlVideoRenderer(1, video_track, m_alCallback,
                                            AlVideoRenderer::LOCAL));

  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
      peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);

  stream->AddTrack(audio_track);
  stream->AddTrack(video_track);
  if (!m_peerConnection->AddStream(stream)) {
  }
  typedef std::pair<std::string,
                    rtc::scoped_refptr<webrtc::MediaStreamInterface>>
      MediaStreamPair;
  m_activeStreams.insert(MediaStreamPair(stream->label(), stream));
}

void Conductor::queueUIThreadCallback(int msg_id, void *data) {
  /*
   * TODO: union this 2
   */
  UIThreadCallback(msg_id, data);
}

void Conductor::UIThreadCallback(int msg_id, void *data) {
  if (m_debug) {
  }
  switch (msg_id) {
  case PEER_CONNECTION_CLOSED:
    DeletePeerConnection();
    RTC_DCHECK(m_activeStreams.empty());
    break;
  case SEND_LOCAL_CANDIDATE: {
    std::string *msg = reinterpret_cast<std::string *>(data);
    if (msg) {
      m_pendingMessages.push_back(msg);
    }

    if (!m_pendingMessages.empty()) {
      msg = m_pendingMessages.front();
      m_pendingMessages.pop_front();
      m_alCallback->onCandidateCb(*msg);
    }
    delete msg;
  } break;
  case SEND_LOCAL_SDP: {
    std::string *msg = reinterpret_cast<std::string *>(data);
    if (msg) {
      m_pendingMessages.push_back(msg);
    }

    if (!m_pendingMessages.empty()) {
      msg = m_pendingMessages.front();
      m_pendingMessages.pop_front();
      m_alCallback->onSdpCb(*msg);
    }
    delete msg;
  } break;
  case SEND_MESSAGE_TO_PEER: {
    if (m_debug) {
    }
  } break;

  case NEW_STREAM_ADDED: {
    webrtc::MediaStreamInterface *stream =
        reinterpret_cast<webrtc::MediaStreamInterface *>(data);
    webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
    // Only render the first track.
    if (!tracks.empty()) {
      webrtc::VideoTrackInterface *track = tracks[0];
      m_remoteRenderer.reset(
          new AlVideoRenderer(1, track, m_alCallback, AlVideoRenderer::REMOTE));
    }
    stream->Release();
    break;
  }

  case STREAM_REMOVED: {
    // Remote peer stopped sending a stream.
    webrtc::MediaStreamInterface *stream =
        reinterpret_cast<webrtc::MediaStreamInterface *>(data);
    stream->Release();
    break;
  }

  default:
    RTC_DCHECK(false);
    break;
  }
}

// **
// * CreateSessionDescriptionObserver implementation.
// *
void Conductor::OnSuccess(webrtc::SessionDescriptionInterface *desc) {
  if (m_debug) {
  }
  m_peerConnection->SetLocalDescription(
      DummySetSessionDescriptionObserver::Create(), desc);

  std::string sdp;
  desc->ToString(&sdp);
  Json::StyledWriter writer;
  Json::Value jmessage;
  jmessage[kSessionDescriptionTypeName] = desc->type();
  jmessage[kSessionDescriptionSdpName] = sdp;
  //  TODO move to callback
  // SendMessage(writer.write(jmessage));
  std::string *msg = new std::string(writer.write(jmessage));
  queueUIThreadCallback(SEND_LOCAL_SDP, msg);
}

void Conductor::OnFailure(const std::string &error) {}

void Conductor::SendMessage(const std::string &json_object) {
  // std::string *msg = new std::string(json_object);
  // queueUIThreadCallback(SEND_MESSAGE_TO_PEER, msg);
}

void Conductor::setImageData(uint8_t *pImageBytes, size_t len, int width,
                             int height) {
  // m_dataManager->setImageData(pImageBytes, len, width, height);
}

void Conductor::setImageData(std::vector<unsigned char> imageBytes, int width,
                             int height) {
  // m_dataManager->m_alVideoCapturer->setImageData(imageBytes, width, height);
}
