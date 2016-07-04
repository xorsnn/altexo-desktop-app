#ifndef ALWSINTERFACE_H
#define ALWSINTERFACE_H

#include <string>

class AlCallback;

class AlWsClientInterface {
public:
  virtual ~AlWsClientInterface() {}
  virtual void init(AlCallback *alCallback) = 0;
};
//]

#endif // ALWSINTERFACE_H
