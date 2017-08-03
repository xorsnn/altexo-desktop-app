
#ifndef ALSETTINGS_H
#define ALSETTINGS_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include "al_utils_export.h"

// class to read and store settings in external file
using boost::property_tree::ptree;

class AL_UTILS_EXPORT AlSettings {

public:
  AlSettings();
  ~AlSettings();

  void set(std::string path, std::string value);
  std::string get(std::string path);

private:
  void load(const std::string &filename);

  std::string m_filename; // log filename
  ptree m_pt;
};

#endif // SETTINGS_H
