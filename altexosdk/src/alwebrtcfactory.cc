#include "AL_API/alwebrtcpluginapi.hpp"
#include "alsdkplugin.hpp"
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
#include <iostream>

class AlWebRtcFactory : public AlWebRtcPluginApi {
public:
  AlWebRtcFactory() {}

  std::string name() const { return "AlWebRtcFactory"; }

  AlSdkAPI *createSdkApi() { return new AlSdkPlugin(); }

  ~AlWebRtcFactory() {}
};

extern "C" BOOST_SYMBOL_EXPORT AlWebRtcFactory plugin;
AlWebRtcFactory plugin;
