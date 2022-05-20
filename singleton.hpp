#include "pthread.h"

template <typename T>
class singleton {
public:
    static T& Instance(){
        if(s_instance == nullptr){
            if (pthread_mutex_trylock(&lock) != 0){return;}
            if(s_instance == nullptr){
                s_instance = new T();
            }
        }
        pthread_mutex_unlock(&lock);
        return *s_instance;
    }
    void Destroy(){
        delete s_instance;
        s_instance = nullptr;
    }
    singleton(singleton const&)      = delete;           /*Override*/
    void operator=(singleton const&) = delete;           /*Override*/
private:
    static T* s_instance;
    static pthread_mutex_t lock;

protected:
    singleton() : pthread_mutex_init(&lock, NULL){
    }
};
