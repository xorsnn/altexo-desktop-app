#include "alconnclient.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

std::string SERVER_LINK = "https://dev.lugati.ru";

AlConnClient::AlConnClient() {
  std::cout << "constructor" << std::endl;
  m_token = "";
  m_peerId = "";
  // login("Galya", "ujnm567");
}

AlConnClient::~AlConnClient() {}

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
    std::cout << "me" << std::endl;

    // requesting ws connection parameters
    auto future_text = cpr::PostCallback(
        boost::bind(&AlConnClient::handleHttpResponse, this, _1,
                    HTTP_INIT_WS_CONN),
        cpr::Url{SERVER_LINK + "/chat/api/join/"}, cpr::Payload{},
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

    std::cout << m_wssLink << std::endl;

    // TODO: seems like port is not nesessary
    m_WsCl.init(m_wssLink, 8888, this);
    m_WsCl.run();
    break;
  }
  default:
    std::cout << "unknown http response" << std::endl;
  }
}

void AlConnClient::onWsMessageCb(std::vector<char> msg) {
  std::string msgStr(msg.begin(), msg.end());
  boost::property_tree::ptree pt;
  std::stringstream ss(msgStr);
  boost::property_tree::read_json(ss, pt);
  std::string action = pt.get<std::string>("action");
  std::cout << action << std::endl;
  if (action == "logged_in") {
    m_peerId = pt.get<std::string>("data.id");
    std::cout << m_peerId << std::endl;
    //   Q_EMIT OnSignedInSignal();
  } else if (action == "update_user_list") {
  } else if (action == "message_from_peer") {
  }
  // } else if (action == "peer_connected") {
  //   this->m_peers[jsonObj["id"].toString()] = jsonObj["name"].toString();
  //   Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(),
  //                                jsonObj["name"].toString());
  // } else if (action == "message_from_peer") {
  //   QString tmp_msg = jsonObj["data"].toObject()["message"].toString();
  //   QJsonDocument tmp_doc = QJsonDocument::fromJson(tmp_msg.toUtf8());
  //   QJsonObject tmp_jsonObj = tmp_doc.object();
  //   if (tmp_jsonObj.contains("callAccepted")) {
  //     Q_EMIT ConnectToPeerSignal(
  //         jsonObj["data"].toObject()["sender_id"].toString());
  //   } else {
  //     Q_EMIT OnMessageFromPeerSignal(
  //         jsonObj["data"].toObject()["sender_id"].toString().toStdString(),
  //         jsonObj["data"].toObject()["message"].toString().toStdString());
  //   }
  //
  // } else if (action == "update_user_list") {
  //   QJsonArray contactsOnline = jsonObj["data"].toArray();
  //   for (int i = 0; i < contactsOnline.size(); i++) {
  //     if (contactsOnline[i].toObject()["id"].toString() != m_connId) {
  //       this->m_peers[contactsOnline[i].toObject()["id"].toString()] =
  //           contactsOnline[i].toObject()["name"].toString();
  //     }
  //   }
  //   Q_EMIT OnPeerConnectedSignal(jsonObj["id"].toString(),
  //                                jsonObj["name"].toString());
  // } else {
  //   //        Q_EMIT this->onTextMessageReceivedSignal(message);
  // }
}
