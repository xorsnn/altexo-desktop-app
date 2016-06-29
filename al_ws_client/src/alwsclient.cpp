#include "alwsclient.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

std::string SERVER_LINK = "https://dev.lugati.ru";

AlWsClient::AlWsClient() {
  std::cout << "constructor" << std::endl;
  m_token = "";
  login("Galya", "ujnm567");
}

AlWsClient::~AlWsClient() {

}

void AlWsClient::login(std::string login, std::string password) {
  auto future_text = cpr::PostCallback(boost::bind(&AlWsClient::handleHttpResponse, this, _1, HTTP_LOGIN),
  cpr::Url{SERVER_LINK + "/users/auth/login/"},
  cpr::Payload{{"username", login}, {"password", password}});
}

void AlWsClient::handleHttpResponse(cpr::Response r, int responseType) {
  switch ( responseType )
  {
    case HTTP_LOGIN: {
      std::cout << "login" << std::endl;

      boost::property_tree::ptree pt;
      std::stringstream ss(r.text);
      boost::property_tree::read_json(ss, pt);
      m_token = pt.get<std::string>("auth_token");

      // requesting user info
      auto future_text = cpr::GetCallback(boost::bind(&AlWsClient::handleHttpResponse, this, _1, HTTP_ME),
      cpr::Url{SERVER_LINK + "/users/auth/me/"},
      cpr::Header{{"Authorization", "Token " + m_token}});

      break;
    }
    case HTTP_ME: {
      std::cout << "me" << std::endl;

      // requesting ws connection parameters
      auto future_text = cpr::PostCallback(boost::bind(&AlWsClient::handleHttpResponse, this, _1, HTTP_INIT_WS_CONN),
      cpr::Url{SERVER_LINK + "/chat/api/join/"},
      cpr::Payload{},
      cpr::Header{{"User-Agent", "Altexo v0.1"}},
      cpr::Header{{"X-Custom-User-Agent", "Altexo v0.1"}},
      cpr::Header{{"Content-Type", "application/json"}},
      cpr::Header{{"Content-Length", "0"}},
      cpr::Header{{"Authorization", "Token " + m_token}});

      break;
    }
    case HTTP_INIT_WS_CONN: {
      std::cout << "ws conn" << std::endl;

      boost::property_tree::ptree pt;
      std::stringstream ss(r.text);
      boost::property_tree::read_json(ss, pt);
      m_wssLink = pt.get<std::string>("params.wss_url");

      break;
    }
    default:
    std::cout << "unknown http response" << std::endl;
  }
}
