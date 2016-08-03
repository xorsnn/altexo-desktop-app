/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "conductor/conductor.h"
#include "AL_API/sdk_api.hpp"
#include "alvideocapturer.h"
#include "webrtc/api/test/fakeconstraints.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"
#include <iostream>
#include <utility>
#include <vector>
//#include "webrtc/examples/peerconnection/client/defaults.h"

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
  virtual void OnSuccess() { LOG(INFO) << __FUNCTION__; }
  virtual void OnFailure(const std::string &error) {
    LOG(INFO) << __FUNCTION__ << " " << error;
  }

protected:
  DummySetSessionDescriptionObserver() {}
  ~DummySetSessionDescriptionObserver() {}
};

Conductor::Conductor(PeerConnectionClient *client, AlCallback *alCallback,
                     AlDataManager *alDataManager)
    : loopback_(false), m_client(client), m_alCallback(alCallback),
      m_debug(true), m_processingMsg(false), m_isAcceptingConnection(false) {

  m_dataManager = alDataManager;
  //    m_dataManager->setConductor(this);
  //    m_client->RegisterObserver(this);
  //    m_alCallback->RegisterObserver(this);

  // notify about avaliable cameras
  std::vector<std::string> device_names;
  {
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo(0));
    if (info) {
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
  }
  m_alCallback->onDevicesListChangedCb(device_names);
}

Conductor::~Conductor() { ASSERT(m_peerConnection.get() == NULL); }

bool Conductor::connection_active() const {
  return m_peerConnection.get() != NULL;
}

void Conductor::Close() {
  //    TODO move to callback
  //    m_client->SignOut();
  DeletePeerConnection();
}

bool Conductor::InitializePeerConnection() {
  if (m_debug) {
    std::cout << "Conductor::InitializePeerConnection" << std::endl;
  }
  ASSERT(peer_connection_factory_.get() == NULL);
  ASSERT(m_peerConnection.get() == NULL);

  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();

  if (!peer_connection_factory_.get()) {
    std::cout << "Error, Failed to initialize PeerConnectionFactory"
              << std::endl;
    DeletePeerConnection();
    return false;
  }

  if (!CreatePeerConnection(DTLS_ON)) {
    std::cout << "Error, CreatePeerConnection failed" << std::endl;
    DeletePeerConnection();
  }
  AddStreams();
  if (m_debug) {
    std::cout << "Conductor m_peerConnection:" << std::endl;
    std::cout << m_peerConnection.get() << std::endl;
    // std::cout << (m_peerConnection.get() != NULL) << std::endl;
  }
  return m_peerConnection.get() != NULL;
}

bool Conductor::ReinitializePeerConnectionForLoopback() {
  loopback_ = true;
  rtc::scoped_refptr<webrtc::StreamCollectionInterface> streams(
      m_peerConnection->local_streams());
  m_peerConnection = NULL;
  if (CreatePeerConnection(DTLS_OFF)) {
    for (size_t i = 0; i < streams->count(); ++i)
      m_peerConnection->AddStream(streams->at(i));
    m_peerConnection->CreateOffer(this, NULL);
  }
  return m_peerConnection.get() != NULL;
}

bool Conductor::CreatePeerConnection(bool dtls) {
  if (m_debug) {
    std::cout << "Conductor::CreatePeerConnection" << std::endl;
  }
  ASSERT(peer_connection_factory_.get() != NULL);
  ASSERT(m_peerConnection.get() == NULL);

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

void Conductor::DeletePeerConnection() {
  if (m_debug) {
    std::cout << "Conductor::DeletePeerConnection" << std::endl;
  }
  m_peerConnection = NULL;
  m_activeStreams.clear();
  m_alCallback->stopLocalRendererCb();
  m_alCallback->stopRemoteRendererCb();
  peer_connection_factory_ = NULL;
  loopback_ = false;
}

void Conductor::EnsureStreamingUI() {
  ASSERT(m_peerConnection.get() != NULL);
  m_alCallback->ensureStreamingUICb();
  //    if (m_alCallback->IsWindow()) {
  //        if (m_alCallback->current_ui() != MainWnd::STREAMING)
  //            m_alCallback->SwitchToStreamingUI();
  //    }
}

// **
// * PeerConnectionObserver implementation.
// *

// Called when a remote stream is added
void Conductor::OnAddStream(webrtc::MediaStreamInterface *stream) {
  if (this->m_debug) {
    std::cout << "Conductor::OnAddStream" << std::endl;
  }
  LOG(INFO) << __FUNCTION__ << " " << stream->label();

  stream->AddRef();
  webrtc::VideoTrackVector tracks = stream->GetVideoTracks();

  // Only render the first track.
  if (!tracks.empty()) {
    webrtc::VideoTrackInterface *track = tracks[0];

    // TODO additional method
    // Video track

    //        this->m_alCallback->startRemoteRendererCb(track);
    // std::cout << "============qwe=============" << std::endl;
    m_remoteRenderer.reset(new AlVideoRenderer(1, track, m_alCallback));
    // TODO: remove
    // this->m_dataManager->startRemoteRenderer(track, m_alCallback);

    //        this->SetRemoteMedia(track);
  }
  stream->Release();
  //   TODO REMOVE
  //    stream->AddRef();
  //    queueUIThreadCallback(NEW_STREAM_ADDED,
  //                                     stream);
}

void Conductor::OnRemoveStream(webrtc::MediaStreamInterface *stream) {
  if (this->m_debug) {
    std::cout << "Conductor::OnRemoveStream" << std::endl;
  }
  LOG(INFO) << __FUNCTION__ << " " << stream->label();
  stream->AddRef();
  queueUIThreadCallback(STREAM_REMOVED, stream);
}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {
  if (m_debug) {
    std::cout << "Conductor::OnIceCandidate" << std::endl;
  }
  LOG(INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
  // For loopback test. To save some connecting delay.
  if (loopback_) {
    if (!m_peerConnection->AddIceCandidate(candidate)) {
      LOG(WARNING) << "Failed to apply the received candidate";
    }
    return;
  }

  Json::StyledWriter writer;
  Json::Value jmessage;

  jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
  jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
  std::string sdp;
  if (!candidate->ToString(&sdp)) {
    LOG(LS_ERROR) << "Failed to serialize candidate";
    return;
  }
  jmessage[kCandidateSdpName] = sdp;
  // TODO move to callback
  // std::string *msg = new std::string(writer.write(jmessage));
  std::string *msg = new std::string(writer.write(jmessage));
  queueUIThreadCallback(SEND_LOCAL_CANDIDATE, msg);
  // SendMessage(writer.write(jmessage));
}

////
//// PeerConnectionClientObserver implementation.
////

// void Conductor::OnSignedIn() {
//    LOG(INFO) << __FUNCTION__;
//    m_alCallback->switchToPeerListCb(m_client->peers());
//}

// void Conductor::OnDisconnected() {
//    LOG(INFO) << __FUNCTION__;

//    DeletePeerConnection();

//    m_alCallback->onDisconnectedCb();
////    TODO remove when cleaning up
////    if (m_alCallback->IsWindow())
////        m_alCallback->SwitchToConnectUI();
//}

// void Conductor::OnPeerConnected(std::string id, const std::string& name) {
//    LOG(INFO) << __FUNCTION__;
//    // Refresh the list if we're showing it.
//    //    if (m_alCallback->current_ui() == MainWnd::LIST_PEERS)
//    //        m_alCallback->switchToPeerListCb(m_client->peers());
//}

// void Conductor::OnPeerDisconnected(std::string id) {
//    LOG(INFO) << __FUNCTION__;
//    if (id == peer_id_) {
//        LOG(INFO) << "Our peer disconnected";
//        queueUIThreadCallback(PEER_CONNECTION_CLOSED, NULL);
//    }
//    //TODO remove when cleaning up
////    else {
//        // Refresh the list if we're showing it.
////        if (m_alCallback->current_ui() == MainWnd::LIST_PEERS)
////            m_alCallback->switchToPeerListCb(m_client->peers());
////    }
//}

void Conductor::OnMessageFromPeer(std::string peer_id,
                                  std::vector<char> msgVec) {
  std::string message(msgVec.begin(), msgVec.end());
  if (this->m_debug) {
    std::cout << "Conductor::OnMessageFromPeer" << std::endl;
    std::cout << msgVec.size() << std::endl;
    std::cout << message << std::endl;
  }
  //    ASSERT(peer_id_ == peer_id || peer_id_ == "-1");
  ASSERT(!message.empty());

  if (!m_peerConnection.get()) {
    //        ASSERT(peer_id_ == "-1");
    //        TODO remove when cleaning up
    // std::cout << "< 11" << std::endl;
    // std::cout << peer_id << std::endl;
    //        peer_id_ = peer_id;
    // TODO HACK!
    this->m_isAcceptingConnection = true;

    std::cout << "< 11" << std::endl;
    if (!InitializePeerConnection()) {
      std::cout << "< 12" << std::endl;
      LOG(LS_ERROR) << "Failed to initialize our PeerConnection instance";
      std::cout << "Failed to initialize our PeerConnection instance"
                << std::endl;
      //            TODO see
      //            m_client->SignOut();
      return;
    }
    std::cout << "< 13" << std::endl;
  }
  //    else if (peer_id != peer_id_) {
  //        ASSERT(peer_id_ != "-1");
  //        LOG(WARNING) << "Received a message from unknown peer while already
  //        in a "
  //                        "conversation with a different peer.";
  //        std::cout << "Received a message from unknown peer while already in
  //        a "
  //                    "conversation with a different peer." << std::endl;
  //        return;
  //    }

  if (this->m_debug) {
    std::cout << "< 10" << std::endl;
  }

  Json::Reader reader;
  Json::Value jmessage;
  // TODO parced QString
  if (!reader.parse(message, jmessage)) {
    //        LOG(WARNING) << "Received unknown message. " << message;
    std::cout << "Received unknown message. " << message << std::endl;
    return;
  }
  std::string type;
  std::string json_object;

  rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName, &type);
  if (!type.empty()) {
    //        if (type == "offer-loopback") {
    //            // This is a loopback call.
    //            // Recreate the peerconnection with DTLS disabled.
    //            if (!ReinitializePeerConnectionForLoopback()) {
    //                LOG(LS_ERROR) << "Failed to initialize our PeerConnection
    //                instance";
    //                std::cout << "Failed to initialize our PeerConnection
    //                instance" << std::endl;
    //                DeletePeerConnection();
    ////                TODO take a look
    ////                m_client->SignOut();
    //            }
    //            return;
    //        }

    std::string sdp;
    if (!rtc::GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName,
                                      &sdp)) {
      LOG(WARNING) << "Can't parse received session description message.";
      std::cout << "Can't parse received session description message."
                << std::endl;
      return;
    }
    webrtc::SdpParseError error;
    webrtc::SessionDescriptionInterface *session_description(
        webrtc::CreateSessionDescription(type, sdp, &error));
    if (!session_description) {
      LOG(WARNING) << "Can't parse received session description message. "
                   << "SdpParseError was: " << error.description;
      std::cout << "Can't parse received session description message. "
                << "SdpParseError was: " << error.description << std::endl;
      return;
    }
    //        LOG(INFO) << " Received session description :" << message;
    std::cout << " Received session description :" << message << std::endl;
    m_peerConnection->SetRemoteDescription(
        DummySetSessionDescriptionObserver::Create(), session_description);
    if (session_description->type() ==
        webrtc::SessionDescriptionInterface::kOffer) {
      std::cout << "m_peerConnection->CreateAnswer(this, NULL);" << std::endl;
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
      LOG(WARNING) << "Can't parse received message.";
      std::cout << "Can't parse received message." << std::endl;
      std::cout << message << std::endl;
      return;
    }
    webrtc::SdpParseError error;
    google_breakpad::scoped_ptr<webrtc::IceCandidateInterface> candidate(
        webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp, &error));
    if (!candidate.get()) {
      LOG(WARNING) << "Can't parse received candidate message. "
                   << "SdpParseError was: " << error.description;
      std::cout << "Can't parse received candidate message. "
                << "SdpParseError was: " << error.description << std::endl;
      return;
    }
    if (!m_peerConnection->AddIceCandidate(candidate.get())) {
      LOG(WARNING) << "Failed to apply the received candidate";
      std::cout << "Failed to apply the received candidate" << std::endl;
      return;
    }
    //        LOG(INFO) << " Received candidate :" << message;
    std::cout << " Received candidate :" << message << std::endl;
    return;
  }
}

// void Conductor::OnMessageSent(int err) {
//    // Process the next pending message if any.
//    queueUIThreadCallback(SEND_MESSAGE_TO_PEER, NULL);
//}

// void Conductor::OnServerConnectionFailure() {
//    if (this->m_debug) {
//        std::cout << "Error " << "Failed to connect to " << server_ <<
//        std::endl;
//    }
////    TODO remove when cleaning up
////    m_alCallback->MessageBox("Error", ("Failed to connect to " +
/// server_).c_str(),
////                          true);
//}

////=====================
// void Conductor::OnSignedInSlot() {
//    if (this->m_debug) {
//        std::cout << "Conductor::OnSignedInSlot" << std::endl;
//    }
//    this->OnSignedIn();
//}

// void Conductor::OnDisconnectedSlot() {
//    this->OnDisconnected();
//}

////void Conductor::OnPeerConnectedSlot(QString id, QString name) {
////    this->OnPeerConnected(id, name);
////}

// void Conductor::OnPeerDisconnectedSlot(std::string id) {
//    this->OnPeerDisconnected(id);
//}

//~=====================

cricket::VideoCapturer *Conductor::OpenVideoCaptureDevice() {
  if (m_debug) {
    std::cout << "Conductor::OpenVideoCaptureDevice" << std::endl;
    // std::cout << m_dataManager->m_desiredVideoSource << std::endl;
  }
  if (m_dataManager->m_desiredVideoSource ==
      AlSdkAPI::DesiredVideoSource::IMG_SNAPSHOTS) {
    //  sending image snapshots
    if (m_debug) {
      std::cout << "sending snapshots" << std::endl;
    }
    AlVideoCapturer *capturer = new AlVideoCapturer();
    m_dataManager->startVideoCapturer(capturer);

    //        capturer = this->videoCapturer_;
    //        qDebug() << "capturer = " << capturer;

    return capturer;
  } else {
    std::vector<std::string> device_names;
    {
      std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
          webrtc::VideoCaptureFactory::CreateDeviceInfo(0));
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
    cricket::VideoCapturer *capturer = nullptr;

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
    std::cout << "Conductor::AddStreams" << std::endl;
  }
  if (m_activeStreams.find(kStreamLabel) != m_activeStreams.end()) {
    std::cout << "Already added!!!!!!!!" << std::endl;
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
  m_localRenderer.reset(new AlLocalVideoRenderer(1, video_track, m_alCallback));

  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
      peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);

  stream->AddTrack(audio_track);
  stream->AddTrack(video_track);
  if (!m_peerConnection->AddStream(stream)) {
    LOG(LS_ERROR) << "Adding stream to PeerConnection failed";
    std::cout << "Adding stream to PeerConnection failed" << std::endl;
  }
  typedef std::pair<std::string,
                    rtc::scoped_refptr<webrtc::MediaStreamInterface>>
      MediaStreamPair;
  m_activeStreams.insert(MediaStreamPair(stream->label(), stream));
  m_alCallback->SwitchToStreamingUI();
}

// void Conductor::DisconnectFromCurrentPeer() {
//    LOG(INFO) << __FUNCTION__;
//    if (m_peerConnection.get()) {
//        //        m_client->SendHangUp(peer_id_);
//        Q_EMIT SendHangUpSignal(peer_id_);
//        DeletePeerConnection();
//    }

//    if (m_alCallback->IsWindow())
//        m_alCallback->switchToPeerListCb(m_client->peers());
//}
void Conductor::queueUIThreadCallback(int msg_id, void *data) {
  UIThreadCallback(msg_id, data);
}

void Conductor::UIThreadCallback(int msg_id, void *data) {
  if (m_debug) {
    std::cout << "Conductor::UIThreadCallback" << std::endl;
  }
  switch (msg_id) {
  case PEER_CONNECTION_CLOSED:
    LOG(INFO) << "PEER_CONNECTION_CLOSED";
    DeletePeerConnection();
    ASSERT(m_activeStreams.empty());
    break;
  case SEND_LOCAL_CANDIDATE: {
    std::string *msg = reinterpret_cast<std::string *>(data);
    if (msg) {
      m_pendingMessages.push_back(msg);
    }
    if (!m_pendingMessages.empty() && !m_client->IsSendingMessage()) {
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
    if (!m_pendingMessages.empty() && !m_client->IsSendingMessage()) {
      msg = m_pendingMessages.front();
      m_pendingMessages.pop_front();
      m_alCallback->onSdpCb(*msg);
    }
    delete msg;
  } break;
  case SEND_MESSAGE_TO_PEER: {
    LOG(INFO) << "SEND_MESSAGE_TO_PEER";
    if (m_debug) {
      std::cout << "SEND_MESSAGE_TO_PEER" << std::endl;
    }
    // std::string *msg = reinterpret_cast<std::string *>(data);
    // if (msg) {
    //   // For convenience, we always run the message through the queue.
    //   // This way we can be sure that messages are sent to the server
    //   // in the same order they were signaled without much hassle.
    //   m_pendingMessages.push_back(msg);
    // }
    //
    // if (!m_pendingMessages.empty() && !m_client->IsSendingMessage()) {
    //   msg = m_pendingMessages.front();
    //   m_pendingMessages.pop_front();
    //   // TODO hangle failed sending
    //   std::cout
    //       << "Q_EMIT SendToPeerSignal(peer_id_,
    //       QString::fromStdString(*msg));"
    //       << std::endl;
    //   m_alCallback->sendToPeerCb(*msg);
    //   //            Q_EMIT SendToPeerSignal(peer_id_,
    //   //            QString::fromStdString(*msg));
    //   //            if (!m_client->SendToPeer(peer_id_, *msg) && peer_id_ !=
    //   //            "-1") {
    //   //                LOG(LS_ERROR) << "SendToPeer failed";
    //   //                DisconnectFromServer();
    //   //            }
    //   delete msg;
    // }
  } break;

  case NEW_STREAM_ADDED: {
    webrtc::MediaStreamInterface *stream =
        reinterpret_cast<webrtc::MediaStreamInterface *>(data);
    webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
    // Only render the first track.
    if (!tracks.empty()) {
      webrtc::VideoTrackInterface *track = tracks[0];
      //            m_alCallback->startRemoteRendererCb(track);
      m_remoteRenderer.reset(new AlVideoRenderer(1, track, m_alCallback));
      // TODO: remove
      // m_dataManager->startRemoteRenderer(track, m_alCallback);
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
    ASSERT(false);
    break;
  }
}

// **
// * CreateSessionDescriptionObserver implementation.
// *
void Conductor::OnSuccess(webrtc::SessionDescriptionInterface *desc) {
  if (m_debug) {
    std::cout << "Conductor::OnSuccess" << std::endl;
  }
  m_peerConnection->SetLocalDescription(
      DummySetSessionDescriptionObserver::Create(), desc);

  std::string sdp;
  desc->ToString(&sdp);

  // For loopback test. To save some connecting delay.
  if (loopback_) {
    // Replace message type from "offer" to "answer"
    webrtc::SessionDescriptionInterface *session_description(
        webrtc::CreateSessionDescription("answer", sdp, nullptr));
    m_peerConnection->SetRemoteDescription(
        DummySetSessionDescriptionObserver::Create(), session_description);
    return;
  }

  Json::StyledWriter writer;
  Json::Value jmessage;
  jmessage[kSessionDescriptionTypeName] = desc->type();
  jmessage[kSessionDescriptionSdpName] = sdp;
  //  TODO move to callback
  // SendMessage(writer.write(jmessage));
  std::string *msg = new std::string(writer.write(jmessage));
  queueUIThreadCallback(SEND_LOCAL_SDP, msg);
}

void Conductor::OnFailure(const std::string &error) {
  if (m_debug) {
    std::cout << "Conductor::OnFailure" << std::endl;
  }
  LOG(LERROR) << error;
}

void Conductor::SendMessage(const std::string &json_object) {
  std::string *msg = new std::string(json_object);
  queueUIThreadCallback(SEND_MESSAGE_TO_PEER, msg);
}

void Conductor::setImageData(uint8_t *pImageBytes, size_t len, int width,
                             int height) {
  m_dataManager->setImageData(pImageBytes, len, width, height);
}

void Conductor::setImageData(std::vector<unsigned char> imageBytes, int width,
                             int height) {
  m_dataManager->m_alVideoCapturer->setImageData(imageBytes, width, height);
}
