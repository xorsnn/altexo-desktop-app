
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "allogger.hpp"

#include "ui/mainwindow.hpp"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace boost::log::trivial;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
    al_logger, boost::log::sources::severity_logger<severity_level>);

void initLogging() {
  logging::add_file_log(keywords::file_name = "_sample_%N.log",
                        keywords::rotation_size = 10 * 1024 * 1024,
                        keywords::time_based_rotation =
                            sinks::file::rotation_at_time_point(0, 0, 0),
                        keywords::auto_flush = true,
                        keywords::format = "[%TimeStamp%]: %Message%");

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::debug);
}

int main(int, char **) {
  initLogging();
  logging::add_common_attributes();
  boost::log::sources::severity_logger<severity_level> &lg = al_logger::get();

  MainWindow w;
  w.init();
  w.run();

  BOOST_LOG_SEV(lg, debug) << "FIN!";
  return 0;
}
