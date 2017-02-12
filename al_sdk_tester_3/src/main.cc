#include "AL_API/alwebrtcpluginapi.hpp"
#include <boost/dll/import.hpp> // for import_alias
#include <iostream>

namespace dll = boost::dll;

int main(int argc, char *argv[]) {
  std::cout << "yahoo!" << std::endl;

  boost::filesystem::path lib_path(
      // "/home/xors/workspace/ALTEXO/altexo/al_sdk_testlib/build");
      "/home/xors/workspace/lib/webrtc-checkout/src/out/Default");

  boost::shared_ptr<AlWebRtcPluginApi> plugin;
  std::cout << "Loading the plugin" << std::endl;

  plugin = dll::import<AlWebRtcPluginApi>(lib_path / "libpeerconnection_2.so",
                                          "plugin",
                                          dll::load_mode::append_decorations);

  // =============================================
  AlSdkAPI *sdkApi = plugin->createSdkApi();
  std::cout << " >>> " << std::endl;
  std::cout << sdkApi << std::endl;
  sdkApi->init2();
  delete sdkApi;
  std::cout << " >>> " << std::endl;
  // =============================================
  return 0;
}
