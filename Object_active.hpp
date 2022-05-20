#include "Deque.hpp"

typedef struct ObjectActive{
    Deque* queue;
    void (*treat)(void*);      /*Treats each element in the queue*/
    void (*end_treat)();       /*Function is called after treating each element*/
    pthread_t private_thread;
} AO, *AO_ptr;

//*****************************/
//========== Methods ==========/
//*****************************/
AO_ptr newAO(Deque* queue, void (*threat)(void*), void (*end_threat)());
void destroyAO(AO_ptr AO);
