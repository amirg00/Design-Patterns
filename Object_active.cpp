#include "Object_active.hpp"

void newAO(Deque* queue, void (*treat)(void*), void (*end_treat)()){

}
void destroyAO(AO_ptr AO){
    destoryQ(AO->queue);
}

void eventExecute(AO_ptr AO){
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

