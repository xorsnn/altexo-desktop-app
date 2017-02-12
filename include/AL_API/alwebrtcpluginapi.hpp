
#include "sdk_api.hpp"
#include <string>

class AlWebRtcPluginApi {
public:
  virtual std::string name() const = 0;
  virtual AlSdkAPI *createSdkApi() = 0;

  virtual ~AlWebRtcPluginApi() {}
};
