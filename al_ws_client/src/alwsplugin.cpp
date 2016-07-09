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
  newMessageSignal("one", "two"); // testing
}
