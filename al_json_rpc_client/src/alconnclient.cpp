#include "alconnclient.hpp"
#include "contact.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// TODO: move to config file
std::string SERVER_WS_LINK = "wss://signal.altexo.com";

AlConnClient::AlConnClient(AlWsCb *alWsCb) : m_debug(true) {
  std::cout << "constructor" << std::endl;
  m_alWsCb = alWsCb;
  m_token = "";
}

AlConnClient::~AlConnClient() { std::cout << "destructor" << std::endl; }

void AlConnClient::initWsConnection() {
  m_wssLink = SERVER_WS_LINK + "/al_chat";

  if (m_debug) {
    std::cout << m_wssLink << std::endl;
  }

  m_wsCl.init(m_wssLink, m_alWsCb);
  m_wsCl.run();
}
