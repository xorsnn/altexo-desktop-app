#ifndef ALPLUGINTEST_H
#define ALPLUGINTEST_H

#include "AL_API/al_plugin_test_api.hpp"
// #include "altextmessage.hpp"
// #include <boost/config.hpp>
#include <boost/dll/alias.hpp>
#include <boost/shared_ptr.hpp>
#include "boost/make_shared.hpp"
// #include <string>

class AlPluginTest : public AlPluginTestAPI {
public:
  AlPluginTest();
  ~AlPluginTest();

  // api
  void testMethod();

  // Factory method
  static boost::shared_ptr<AlPluginTestAPI> create() {
    return boost::make_shared<AlPluginTest>();
    // return boost::shared_ptr<AlPluginTest>(new AlPluginTest());
  }
};

// extern "C" BOOST_SYMBOL_EXPORT AlPluginTest plugin;
// AlPluginTest plugin;
BOOST_DLL_ALIAS(
    AlPluginTest::create, // <-- this function is exported with...
    create_plugin                               // <-- ...this alias name
)

#endif // ALPLUGINTEST_H
