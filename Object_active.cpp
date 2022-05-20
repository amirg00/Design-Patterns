#include "Object_active.hpp"

AO_ptr newAO(Deque* queue, void (*treat)(void*), void (*end_treat)()){
    AO_ptr newAO = (AO_ptr) (malloc(sizeof(AO)));
    newAO->queue = queue;
    newAO->treat = treat;
    newAO->end_treat = end_treat;

    // Traverse elements without remove from queue
    deque_node_ptr curr = queue->head;
    while (curr != nullptr){
        treat(curr->data);
        curr = curr->next;
    }
    end_treat();
    return newAO;
}
void destroyAO(AO_ptr AO){
    destoryQ(AO->queue);
}
