#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct NodeDeque{
    void* data;
    struct NodeDeque *next;
    struct NodeDeque *prev;
} deque_node, *deque_node_ptr;

typedef struct Deque{
    int size;
    deque_node_ptr head;
    deque_node_ptr tail;
}Deque;

//*****************************/
//========== Methods ==========/
//*****************************/
Deque* createQ();
void enQ(Deque* queue, void* data);
void* DEQUEUE_TAIL(Deque* queue);  /*Original dequeue head*/
void* deQ(Deque* queue);           /*Dequeue head*/
int isEmpty(Deque* stack);
void destoryQ(Deque* queue);
int size(Deque* queue);
void print_queue(Deque* stack);