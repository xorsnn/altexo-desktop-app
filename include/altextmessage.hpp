
#ifndef ALTEXTMESSAGE_H
#define ALTEXTMESSAGE_H
#include <string>
#include <vector>

using alMsg = std::vector<char>;

class AlTextMessage {
public:
  AlTextMessage(std::string msg) { m_message = alMsg(msg.begin(), msg.end()); }

  // TODO avoid  in the future
  AlTextMessage(alMsg msg) { m_message = msg; }

  // TODO: remove, deprecated
  std::string getText() {
    return std::string(m_message.begin(), m_message.end());
  }

  std::string toString() {
    return std::string(m_message.begin(), m_message.end());
  }

  void setText(std::string msg) { m_message = alMsg(msg.begin(), msg.end()); }

  // TODO avoid  in the future
  alMsg asCharVector() { return m_message; }
  alMsg toMsg() { return m_message; }

  static std::string msgToString(alMsg msg) {
    return std::string(msg.begin(), msg.end());
  }

  static alMsg stringToMsg(std::string msg) {
    return alMsg(msg.begin(), msg.end());
  }

  static alMsg cStrToMsg(const char *cStr) {
    std::string stdStr(cStr);
    return stringToMsg(stdStr);
  }

  static std::string cStrToString(const char *cStr) {
    std::string stdStr(cStr);
    return stdStr;
  }

  const char *cStr() { return toString().c_str(); }

private:
  alMsg m_message;
};

#endif // ALTEXTMESSAGE_H
