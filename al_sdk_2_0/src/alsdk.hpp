
#ifndef ALSDK_H
#define ALSDK_H

#include "alconductor.hpp"
#include "boost/thread.hpp"

class AlSdk {
public:
  AlSdk();
  ~AlSdk();

  void run();

private:
  void threadMain();

  rtc::scoped_refptr<AlConductor> m_conductor;
  boost::thread *m_internalThread;
};

#endif // ALSDK_H
