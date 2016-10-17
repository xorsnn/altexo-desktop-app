#include "alsettings.hpp"

AlSettings::AlSettings() : m_filename("local_storage.xml") {
  try {
    load(m_filename);
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << "\n";
  }
}

AlSettings::~AlSettings() {}

void AlSettings::set(std::string path, std::string value) {
  m_pt.put(path, value);
  write_xml(m_filename, m_pt);
}

std::string AlSettings::get(std::string path) {
  return m_pt.get<std::string>(path, "");
}

void AlSettings::load(const std::string &filename) {
  read_xml(m_filename, m_pt);
}
