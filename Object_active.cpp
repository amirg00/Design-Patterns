#include "Object_active.hpp"

AO_ptr newAO(Deque* queue, void (*treat)(void*), void (*end_treat)()){
    AO_ptr active_object = (AO_ptr) malloc(sizeof(AO));

    /*Fill Active Object's fields*/
    active_object->treat = treat;
    active_object->end_treat = end_treat;
    active_object->queue = queue;

    // Create rector's thread id
    if (pthread_create(&active_object->private_thread, NULL, eventExecute, active_object) != 0) {
        printf("Unable to create thread\n");
    }
    return active_object;
}

void destroyAO(AO_ptr AO){
    pthread_join(AO->private_thread, NULL); // Wait for thread to finish everything before canceling.
    pthread_cancel(AO->private_thread);
    destoryQ(AO->queue);
    free(AO);
}

void eventExecute(void* AO_input){
    AO_ptr AO = (AO_ptr) AO_input;
    for(;;){
        if (isEmpty(AO->queue)){ // Queue is empty: execute end_threat function
            end_treat();
            continue;
        }
        // Dequeue the latest event to handle.
        // The event's data is sent to threat function.
        deque_node_ptr headEvent = deQ(AO->queue);
        AO->treat(headEvent->data);
    }
}

