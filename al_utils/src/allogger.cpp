#include "allogger.hpp"

AlLogger::AlLogger() {}

AlLogger::~AlLogger() {}

void AlLogger::operator<<(std::string msg) {
  std::cout << "al_log: " << msg << std::endl;
}
