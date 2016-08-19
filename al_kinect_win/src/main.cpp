
#include "alkinectmanager.hpp"
#include <boost/thread/thread.hpp>
#include <iostream>

int main(int, char **) {
  AlKinectManager m;
  m.init();

  while (1) {
    // std::cout << "oper" << std::endl;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(33));
  }
  return 0;
}
