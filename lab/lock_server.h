// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include <map>
#include <pthread.h>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"

class lock_server {

 protected:
  int nacquire;
  std::map<lock_protocol::lockid_t,bool> lockStates;
  pthread_mutex_t lock_mutex;
  pthread_cond_t lock_cv;

 public:
  lock_server();
  ~lock_server(){};
  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status grantLock(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status releaseLock(int clt, lock_protocol::lockid_t lid, int &);

};

#endif 







