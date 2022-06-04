#include "Object_active.hpp"

void newAO(Deque* queue, void (*treat)(void*), void (*end_treat)()){

    // Traverse elements without remove from queue
    deque_node_ptr curr = queue->head;
    while (curr != nullptr){
        treat(curr->data);
        curr = curr->next;
    }
    end_treat();
}
void destroyAO(AO_ptr AO){
    destoryQ(AO->queue);
}
