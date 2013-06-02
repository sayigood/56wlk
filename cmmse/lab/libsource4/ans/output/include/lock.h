/******************************************************************************
 *
 * lock: A wrapper to ensure that a mutex will be unlock in any case.
 *
 * Example:
 * \code
 *      pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 *      foo() {
 *          lock lock(&mutex);
 *          // do something...
 *          if (error) {
 *              return;
 *          }
 *          // do more things...
 *      }
 * \endcode
 *****************************************************************************/
#ifndef _ANS_LOCK_H
#define _ANS_LOCK_H

#include <pthread.h>

namespace ans
{

class lock
{
public:
    lock(pthread_mutex_t *m) : mutex(m) {
        pthread_mutex_lock(mutex);
    }
    ~lock() {
        pthread_mutex_unlock(mutex);
    }
private:
    lock(); // forbidden
    pthread_mutex_t *mutex;
}; // class lock

} // namespace 
#endif //_ANS_LOCK_H
