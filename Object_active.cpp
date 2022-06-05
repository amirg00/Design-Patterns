#include "Object_active.hpp"

AO_ptr newAO(Deque* queue, void (*treat)(void*), void (*end_treat)(void*)){
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
        if (isEmpty(AO->queue)){ // Queue is empty: skip
            continue;
        }
        // Dequeue the latest event to handle.
        // The event's data is sent to threat function.
        // After threat is done, function calls end threat function
        // on the data of the latest event - execute end_threat function.
        deque_node_ptr headEvent = deQ(AO->queue);
        AO->treat(headEvent->data);
        AO->end_treat(headEvent->data);
    }
}

