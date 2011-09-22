// the lock server implementation

#include "lock_server.h"
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
    pthread_mutex_init(&lock_mutex, NULL);
    pthread_cond_init(&lock_cv, NULL);
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

lock_protocol::status
lock_server::grantLock(int clt, lock_protocol::lockid_t lid, int &r)
{

    pthread_mutex_lock(&lock_mutex);
    if(lockStates.find(lid)==lockStates.end())
    {
        this->lockStates[lid]=true;
        pthread_mutex_unlock(&lock_mutex);

    }else 
    {
        while(this->lockStates[lid]==true)
        {
            pthread_cond_wait(&lock_cv, &lock_mutex);
        }
        this->lockStates[lid]=true;
        pthread_mutex_unlock(&lock_mutex);
    }

    lock_protocol::status ret = lock_protocol::OK;
    return ret;
}

lock_protocol::status
lock_server::releaseLock(int clt, lock_protocol::lockid_t lid, int &r)
{
    assert(lockStates[lid]==true);
    pthread_mutex_lock(&lock_mutex);
    this->lockStates[lid]=false;
    pthread_cond_signal(&lock_cv);
    pthread_mutex_unlock(&lock_mutex);
    return lock_protocol::OK;
}



