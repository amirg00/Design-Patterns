#include "Deque.h"

typedef struct ObjectActive{
    Deque* queue;
    void (*treat)(void*);      /*Treats each element in the queue*/
    void (*end_treat)(void*);  /*Function is called after treating each element*/
    pthread_t private_thread;  /*Thread id*/
} AO, *AO_ptr;

//*****************************/
//========== Methods ==========/
//*****************************/
AO_ptr newAO(Deque* queue, void (*treat)(void*), void (*end_treat)(void*));
void destroyAO(AO_ptr AO);
void eventExecute(void* AO);
