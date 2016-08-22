#include "alconnclient.hpp"
#include "contact.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

std::string SERVER_LINK = "https://dev.lugati.ru";
std::string SERVER_WS_LINK = "wss://dev.lugati.ru";

template <typename T>
std::vector<T> as_vector(boost::property_tree::ptree const &pt,
                         boost::property_tree::ptree::key_type const &key) {
  std::vector<T> r;
  for (auto &item : pt.get_child(key))
    r.push_back(item.second.get_value<T>());
  return r;
}

AlConnClient::AlConnClient(AlWsCb *alWsCb) : m_debug(true) {
  std::cout << "constructor" << std::endl;
  m_alWsCb = alWsCb;
  m_token = "";
}

AlConnClient::~AlConnClient() { std::cout << "destructor" << std::endl; }

void AlConnClient::login(std::string login, std::string password) {
  auto future_text = cpr::PostCallback(
      boost::bind(&AlConnClient::handleHttpResponse, this, _1, HTTP_LOGIN),
      cpr::Url{SERVER_LINK + "/users/auth/login/"},
      cpr::Payload{{"username", login}, {"password", password}});
}

void AlConnClient::handleHttpResponse(cpr::Response r, int responseType) {
  switch (responseType) {
  case HTTP_LOGIN: {
    std::cout << "login" << std::endl;

    boost::property_tree::ptree pt;
    std::stringstream ss(r.text);
    boost::property_tree::read_json(ss, pt);
    m_token = pt.get<std::string>("auth_token");

    // requesting user info
    auto future_text = cpr::GetCallback(
        boost::bind(&AlConnClient::handleHttpResponse, this, _1, HTTP_ME),
        cpr::Url{SERVER_LINK + "/users/auth/me/"},
        cpr::Header{{"Authorization", "Token " + m_token}});

    break;
  }
  case HTTP_ME: {
    std::cout << "ws conn" << std::endl;

    if (m_debug) {
      std::cout << r.text << std::endl;
    }

    boost::property_tree::ptree pt;
    std::stringstream ss(r.text);
    boost::property_tree::read_json(ss, pt);

    // TODO: using hardcoded link
    m_wssLink = SERVER_WS_LINK + "/chat";

    if (m_debug) {
      std::cout << m_wssLink << std::endl;
    }

    m_wsCl.init(m_wssLink, m_alWsCb);
    m_wsCl.run();

    // **
    // * Sending authenticate
    // *
    std::ostringstream stream;
    boost::property_tree::ptree ptAuth;
    // boost::property_tree::ptree dataNode;
    // authenticate [ token ] -> boolean
    ptAuth.put("jsonrpc", "2.0");
    ptAuth.put("method", "authenticate");
    ptAuth.put("params", m_token);
    ptAuth.put("id", "2");
    boost::property_tree::write_json(stream, ptAuth, false);
    std::string strJson = stream.str();
    AlTextMessage msgToSend(strJson);
    m_wsCl.sendMessage(msgToSend);

    // std::ostringstream stream1;
    // boost::property_tree::ptree ptAuth1;
    // // boost::property_tree::ptree dataNode;
    // // authenticate [ token ] -> boolean
    // ptAuth1.put("jsonrpc", "2.0");
    // ptAuth1.put("method", "room/open");
    // ptAuth1.put("params", "[\"test111\", \"true\"]");
    // ptAuth1.put("id", "3");
    // boost::property_tree::write_json(stream1, ptAuth1, false);
    // std::string strJson1 = stream1.str();
    // AlTextMessage msgToSend1(strJson1);
    // m_wsCl.sendMessage(msgToSend1);

  } break;
  default:
    std::cout << "unknown http response" << std::endl;
  }
}
