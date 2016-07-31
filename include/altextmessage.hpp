
#ifndef ALTEXTMESSAGE_H
#define ALTEXTMESSAGE_H
#include <string>
#include <vector>

class AlTextMessage {
public:
  AlTextMessage(std::string msg) {
    m_message = std::vector<char>(msg.begin(), msg.end());
  }

  // TODO avoid  in the future
  AlTextMessage(std::vector<char> msg) { m_message = msg; }

  std::string getText() {
    return std::string(m_message.begin(), m_message.end());
  }

  void setText(std::string msg) {
    m_message = std::vector<char>(msg.begin(), msg.end());
  }

  // TODO avoid  in the future
  std::vector<char> asCharVector() { return m_message; }

private:
  std::vector<char> m_message;
};

#endif // ALTEXTMESSAGE_H
