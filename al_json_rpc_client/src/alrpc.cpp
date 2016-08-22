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
  ptAuth.put("params", token);
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
  ptRoomOpen.put("params", "[\"test111\", \"true\"]");
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
  ptRoomOpen.put("params", "[\"brazzers\"]");
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
  std::cout << "AlRpc::onMessage" << std::endl;

  boost::property_tree::ptree pt;
  std::stringstream ss(msg.toString());
  boost::property_tree::read_json(ss, pt);
  long msgId = pt.get<long>("id");
  int msgType = m_requests[msgId];
  m_requests.erase(msgId);
  std::cout << msgId << std::endl;
  switch (msgType) {
  case AlRpcRequest::SERVER_MESSAGE_TYPE::AUTHENTICATE: {
    bool result = pt.get<bool>("result");
    if (result) {
      // roomOpen();
      roomEnter();
    }
  } break;
  case AlRpcRequest::SERVER_MESSAGE_TYPE::ROOM_OPEN: {
    bool result = pt.get<bool>("result");
    // if (result) {
    // roomEnter();
    // }
  } break;
  default: {
    std::cout << "AlRpc unhandled message" << std::endl;
    std::cout << msg.toString() << std::endl;
  }
  }
}
