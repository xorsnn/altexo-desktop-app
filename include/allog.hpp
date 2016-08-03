
#ifndef ALLOG_H
#define ALLOG_H
#include <iostream>
#include <string>

#ifndef AL_DEBUG
bool DEBUG = false;
#else
bool DEBUG = true;
#endif

// void alLog(const std::string msg);
void alLog(const std::string msg) {
  if (DEBUG) {
    std::cout << msg << std::endl;
  }
}

// void alLog(std::string msg);
// class AlTextMessage {
// public:
//   AlTextMessage(std::string msg) {
//     m_message = std::vector<char>(msg.begin(), msg.end());
//   }
//
//   // TODO avoid  in the future
//   AlTextMessage(std::vector<char> msg) { m_message = msg; }
//
//   std::string getText() {
//     return std::string(m_message.begin(), m_message.end());
//   }
//
//   void setText(std::string msg) {
//     m_message = std::vector<char>(msg.begin(), msg.end());
//   }
//
//   // TODO avoid  in the future
//   std::vector<char> asCharVector() { return m_message; }
//
// private:
//   std::vector<char> m_message;
// };

#endif // ALLOG_H
