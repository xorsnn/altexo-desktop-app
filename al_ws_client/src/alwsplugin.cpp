#include "alwsplugin.hpp"
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
  m_connClient = new AlConnClient();
  newMessageSignal.connect(boost::bind(&AlWsCb::onMessageCb, alWsCb, _1, _2));
  m_internalThread = boost::thread(&AlWsPlugin::threadMain, this);
}

void AlWsPlugin::threadMain() {
  m_connClient->login("Galya", "ujnm567");
  // ========================
  std::string peerId = "one";
  std::string message = "two";
  std::vector<char> peerIdVec(peerId.begin(), peerId.end());
  std::vector<char> messageVec(message.begin(), message.end());
  newMessageSignal(peerIdVec, messageVec); // testing
}
