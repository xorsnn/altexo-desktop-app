#include "alrpc.hpp"
#include <boost/foreach.hpp>
#include <iostream>
#include <jsoncpp/json/json.h>

template <typename T>
std::vector<T> as_vector(boost::property_tree::ptree const &pt,
                         boost::property_tree::ptree::key_type const &key) {
  std::vector<T> r;
  for (auto &item : pt.get_child(key))
    r.push_back(item.second.get_value<T>());
  return r;
}

AlRpc::AlRpc() : m_sessionMsgCounter(0) {}

AlRpc::~AlRpc() {}

long AlRpc::getMsgId() {
  m_sessionMsgCounter++;
  return m_sessionMsgCounter;
}

void AlRpc::authenticate(std::string token) {
  // authenticate [ token ] -> boolean
  long reqId = getMsgId();
  m_requests[reqId] = AlRpcRequest::SERVER_MESSAGE_TYPE::AUTHENTICATE;
  std::ostringstream stream;
  boost::property_tree::ptree ptAuth;
  ptAuth.put("jsonrpc", "2.0");
  ptAuth.put("method", "authenticate");

  boost::property_tree::ptree params;
  boost::property_tree::ptree tokenVal;
  tokenVal.put("", token);
  params.push_back(std::make_pair("", tokenVal));

  ptAuth.add_child("params", params);
  ptAuth.put("id", reqId);
  boost::property_tree::write_json(stream, ptAuth, false);
  std::string strJson = stream.str();
  AlTextMessage msgToSend(strJson);
  sendMessage(msgToSend);
}

void AlRpc::roomOpen() {
  // room/open [ name, p2p ] -> boolean
  long reqId = getMsgId();
  m_requests[reqId] = AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_OPEN;
  std::ostringstream stream;
  boost::property_tree::ptree ptRoomOpen;
  ptRoomOpen.put("jsonrpc", "2.0");
  ptRoomOpen.put("method", "room/open");

  boost::property_tree::ptree params;
  boost::property_tree::ptree nameVal;
  nameVal.put("", "1234");
  boost::property_tree::ptree p2pVal;
  p2pVal.put("", true);
  params.push_back(std::make_pair("", nameVal));
  params.push_back(std::make_pair("", p2pVal));
  ptRoomOpen.add_child("params", params);
  ptRoomOpen.put("id", reqId);
  boost::property_tree::write_json(stream, ptRoomOpen, true);
  std::string strJson = stream.str();
  AlTextMessage msgToSend(strJson);
  sendMessage(msgToSend);
}

void AlRpc::roomClose() {}
void AlRpc::roomEnter() {
  // room/enter [ name ] -> boolean
  long reqId = getMsgId();
  m_requests[reqId] = AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_ENTER;
  std::ostringstream stream;
  boost::property_tree::ptree ptRoomOpen;
  ptRoomOpen.put("jsonrpc", "2.0");
  ptRoomOpen.put("method", "room/enter");
  ptRoomOpen.put("params", "1234");
  ptRoomOpen.put("id", reqId);
  boost::property_tree::write_json(stream, ptRoomOpen, false);
  std::string strJson = stream.str();
  AlTextMessage msgToSend(strJson);
  sendMessage(msgToSend);
}
void AlRpc::roomLeave() {}
void AlRpc::roomOffer() {}

void AlRpc::onRoomIceCandidate() {}
void AlRpc::offer() {}
void AlRpc::onIceCandidate() {}

void AlRpc::onMessage(AlTextMessage msg) {
  // std::cout << "AlRpc::onMessage" << std::endl;
  // std::cout << msg.toString() << std::endl;

  boost::property_tree::ptree pt;
  std::stringstream ss(msg.toString());
  boost::property_tree::read_json(ss, pt);

  long msgId = pt.get<long>("id", -1);
  std::string method = pt.get<std::string>("method", "none");
  int msgType = AlRpcRequest::SERVER_MESSAGE_TYPE::NONE;

  // offer notification
  if (method == "offer") {
    msgType = AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_OFFER;
    m_sdpOfferId = msgId;
  }
  // ice-cadidate notification
  else if (method == "ice-candidate") {
    msgType = AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_ICE_CANDIDATE;
  } else {
    std::map<long, int>::iterator it = m_requests.find(msgId);
    if (it != m_requests.end()) {
      msgType = m_requests[msgId];
      m_requests.erase(it);
    }
  }

  switch (msgType) {
  case AlRpcRequest::SERVER_MESSAGE_TYPE::AUTHENTICATE: {
    bool result = pt.get<bool>("result");
    if (result) {
      roomOpen();
    }
  } break;
  case AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_OPEN: {
    bool result = pt.get<bool>("result");
    // if (result) {
    //   roomEnter();
    // }
  } break;
  case AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_OFFER: {
    sdpSignal(msg);
  } break;
  case AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_ICE_CANDIDATE: {
    BOOST_FOREACH (boost::property_tree::ptree::value_type &v,
                   pt.get_child("params")) {
      assert(v.first.empty()); // array elements have no names
      std::ostringstream streamI;
      boost::property_tree::write_json(streamI, v.second, false);
      std::string strJsonI = streamI.str();
      iceCandidateSignal(AlTextMessage(strJsonI));
    }
  } break;
  default: {
    std::cout << "AlRpc unhandled message" << std::endl;
    std::cout << msg.toString() << std::endl;
  }
  }
}

void AlRpc::sendSdpAnswer(AlTextMessage msg) {
  boost::property_tree::ptree pt;
  std::stringstream ss(msg.toString());
  boost::property_tree::read_json(ss, pt);

  std::ostringstream stream;
  Json::Value docVal(Json::objectValue), resVal(Json::arrayValue);
  resVal.append(pt.get<std::string>("sdp"));
  docVal["result"] = resVal;
  docVal["id"] = int(m_sdpOfferId);
  docVal["jsonrpc"] = "2.0";
  stream << docVal;

  sendMessage(AlTextMessage(stream.str()));
}

void AlRpc::sendIceCandidate(AlTextMessage msg) {
  std::cout << "AlRpc::sendIceCandidate" << std::endl;
  std::cout << msg.toString() << std::endl;

  // TODO: add method and all other nesessary fields
  // sendMessage(msg);
  boost::property_tree::ptree pt;
  std::stringstream ss(msg.toString());
  boost::property_tree::read_json(ss, pt);

  std::ostringstream stream;
  boost::property_tree::ptree ptToSend;
  ptToSend.put("jsonrpc", "2.0");
  ptToSend.put("method", "room/ice-candidate");

  boost::property_tree::ptree params;
  params.push_back(std::make_pair("", pt));
  ptToSend.add_child("params", params);
  boost::property_tree::write_json(stream, ptToSend, false);
  std::string strJson = stream.str();
  AlTextMessage msgToSend(strJson);

  std::cout << "sending: " << std::endl;
  std::cout << msgToSend.toString() << std::endl;

  sendMessage(msgToSend);
}
