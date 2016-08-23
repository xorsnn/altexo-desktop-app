#include "alrpc.hpp"
#include <iostream>

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
  nameVal.put("", "123");
  boost::property_tree::ptree p2pVal;
  p2pVal.put("", true);
  params.push_back(std::make_pair("", nameVal));
  params.push_back(std::make_pair("", p2pVal));
  ptRoomOpen.add_child("params", params);
  ptRoomOpen.put("id", reqId);
  boost::property_tree::write_json(stream, ptRoomOpen, false);
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
  ptRoomOpen.put("params", "123");
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

  // std::cout << msgId << std::endl;
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
    // std::cout << "AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_OFFER" <<
    // std::endl;
  } break;
  case AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_ICE_CANDIDATE: {
    // std::cout << "AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_ICE_CANDIDATE"
    //           << std::endl;
  } break;
  default: {
    std::cout << "AlRpc unhandled message" << std::endl;
    std::cout << msg.toString() << std::endl;
  }
  }
}
