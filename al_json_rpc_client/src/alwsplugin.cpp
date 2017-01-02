#include "alwsplugin.hpp"
#include "alwsclient.hpp"
#include <iostream>

using namespace std;

AlWsPlugin::AlWsPlugin() {
  std::cout << "AlWsPlugin constructor" << std::endl;
  m_connClient = NULL;
}

AlWsPlugin::~AlWsPlugin() {
  delete m_connClient;
  m_connClient = NULL;
}

void AlWsPlugin::init(AlWsCb *alWsCb) {
  m_connClient = new AlConnClient(alWsCb);

  // TODO: deprecated
  sendWsMessageToPeerSignal.connect(boost::bind(
      &AlWsClient::sendMessageToPeer, m_connClient->getWsClientRef(), _1, _2));
  sendWsMessageSignal.connect(boost::bind(&AlWsClient::sendMessage,
                                          m_connClient->getWsClientRef(), _1));

  sendSdpAnswerSignal.connect(
      boost::bind(&AlRpc::sendSdpAnswer, m_connClient->getWsClientRef(), _1));
  sendSdpOfferSignal.connect(
      boost::bind(&AlRpc::sendSdpOffer, m_connClient->getWsClientRef(), _1));
  sendIceCandidateSignal.connect(boost::bind(
      &AlRpc::sendIceCandidate, m_connClient->getWsClientRef(), _1));

  // json-rpc
  roomOpenSignal.connect(
      boost::bind(&AlRpc::roomOpen, m_connClient->getWsClientRef(), _1));

  m_internalThread = boost::thread(&AlWsPlugin::threadMain, this);
}

void AlWsPlugin::threadMain() {
  // TODO remove for now
  m_connClient->login("Galya", "ujnm567"); // TODO: move to GUI
}

void AlWsPlugin::sendMessageToPeer(AlTextMessage peerId, AlTextMessage msg) {
  sendWsMessageToPeerSignal(peerId.asCharVector(), msg.asCharVector());
}

void AlWsPlugin::sendMessage(AlTextMessage msg) { sendWsMessageSignal(msg); }

void AlWsPlugin::sendSdpOffer(const char *cMsg) {
  AlTextMessage msg = AlTextMessage::cStrToMsg(cMsg);
  sendSdpOfferSignal(msg);
}

void AlWsPlugin::sendSdpAnswer(AlTextMessage msg) { sendSdpAnswerSignal(msg); }

void AlWsPlugin::sendIceCandidate(AlTextMessage msg) {
  sendIceCandidateSignal(msg);
}

// json-rpc
void AlWsPlugin::roomOpen(AlTextMessage msg) { roomOpenSignal(msg); }
