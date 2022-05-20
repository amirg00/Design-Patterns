#include "guard.hpp"

guard::guard(pthread_mutex_t& lock) {
    _lock = lock;
    pthread_mutex_lock(&_lock);
}

guard::~guard() {
    pthread_mutex_unlock(&_lock);
}