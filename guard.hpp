#include "pthread.h"

class guard {
private:
    pthread_mutex_t _lock{};
public:
    explicit guard(pthread_mutex_t &lock);
    ~guard();
};