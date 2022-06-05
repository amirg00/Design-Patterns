/**
 * Credit to: https://stackoverflow.com/questions/11711920/how-to-implement-multithread-safe-singleton-in-c11-without-using-mutex
 */
#include "pthread.h"
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // lock for the singleton instance

template <typename T>
class singleton {
public:
    static singleton* Instance(T info){
        if(s_instance == nullptr){
            pthread_mutex_lock(&lock); // lock creating new instance
                if(s_instance == nullptr){
                    s_instance = new singleton(info);
                }
        }
        pthread_mutex_unlock(&lock);
        return s_instance;
    }
    void Destroy(){
        pthread_mutex_lock(&lock);  // lock removing
        delete s_instance;
        s_instance = nullptr;
        pthread_mutex_unlock(&lock);
    }
    singleton(singleton const&)      = delete;           /*Override*/
    void operator=(singleton const&) = delete;           /*Override*/
private:
    static singleton* s_instance;
    T _info;                                             /*Instance's info*/
    explicit singleton(T info){
        _info = info;
    }
};
// Here is the static instance
template<typename T>
singleton<T> *singleton<T>::s_instance = nullptr;