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
      break;
    }
    case HTTP_ME: {
      std::cout << "me" << std::endl;
      break;
    }
    case HTTP_INIT_WS_CONN: {
      std::cout << "me" << std::endl;
      break;
    }
    default:
    std::cout << "unknown http response" << std::endl;
  }
  std::cout << r.status_code << std::endl;
  std::cout << r.text << std::endl;
}
