#include "pthread.h"

/**
 * Answer for the guards question is NO!
 */
class guard {
private:
    pthread_mutex_t _lock{};
public:
    explicit guard(pthread_mutex_t &lock);
    ~guard();
};