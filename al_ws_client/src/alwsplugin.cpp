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
  sendWsMessageSignal.connect(boost::bind(
      &AlWsClient::sendMessageToPeer, m_connClient->getWsClientRef(), _1, _2));
  m_internalThread = boost::thread(&AlWsPlugin::threadMain, this);
}

void AlWsPlugin::threadMain() {
  m_connClient->login("Galya", "ujnm567"); // TODO: move to GUI
}

void AlWsPlugin::sendMessageToPeer(AlTextMessage peerId, AlTextMessage msg) {
  sendWsMessageSignal(peerId.asCharVector(), msg.asCharVector());
}
