#include "Deque.hpp"
pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

 Deque* createQ() {
    Deque *queue = (Deque*)malloc(sizeof(Deque));
    if (!queue){
        return NULL;
    }

    // create lock whenever creating queue
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return NULL;
    }

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

// Enqueues to the head of the queue.
void enQ(Deque* queue, void* data){
    pthread_mutex_lock(&lock);
    deque_node_ptr n = (deque_node_ptr)malloc(sizeof(deque_node));
    if (!n){
        pthread_mutex_unlock(&lock);
        return;
    }
    n->data = data;

    if(!queue->size){ /*Queue is empty*/
        n->next = NULL;
        n->prev = NULL;
        queue->head = n;
        queue->tail = n;
        queue->size++;
        pthread_mutex_unlock(&lock);
        return;
    }

    /*Set tail with the new inserted element to the dequeue*/
    queue->tail->prev = n;
    n->next = queue->tail;
    n->prev = NULL;
    queue->tail = n;
    queue->size++;
    pthread_mutex_unlock(&lock);
}


void* deQ(Deque* queue){
    pthread_mutex_lock(&lock);
    if(isEmpty(queue)){
        pthread_cond_wait(&cond, &lock);
    }
    printf("Queue Size: %d\n", size(queue));
    deque_node_ptr head_prev = queue->head->prev;
    void* rm_node_str = queue->head->data;

    //printf("Queue Size: %s\n", head_prev->data);
    free(queue->head); // free dequeued resource
    queue->head = head_prev;
    if(queue->head) {queue->head->next = NULL;}
    else {queue->tail = NULL;}
    queue->size--;
    pthread_mutex_unlock(&lock);
    return rm_node_str;
}

void* DEQUEUE_TAIL(Deque* queue){
    pthread_mutex_lock(&lock);
    if(isEmpty(queue)){
        pthread_cond_wait(&cond, &lock);
    }
    deque_node_ptr rm_node = queue->tail;
    void* rm_node_str = rm_node->data;
    deque_node_ptr tail_next = queue->tail->next;
    if (tail_next) {tail_next->prev = NULL;}
    else{queue->head = NULL;}
    queue->tail = tail_next;
    free(rm_node);
    pthread_mutex_unlock(&lock);
    return rm_node_str;
}

// Destroy stack: clears all the nodes and free the current queue
void destoryQ(Deque* queue){
    pthread_mutex_lock(&lock);
    while (queue->head != nullptr){
        deque_node_ptr curr = queue->head;
        queue->head = queue->head->next;
        free(curr);
    }
    free(queue);
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock); // destroy lock after destroying the queue
}

int size(Deque* queue){
    return queue->size;
}

bool isEmpty(Deque* queue){
    return queue->size == 0;
}

// Print function for debugging.
void print_queue(Deque* queue){
    deque_node_ptr curr = queue->head;
    printf("---- HEAD TO TAIL -----\n");
    while(curr){
        printf("%s ", (char*) curr->data);
        curr = curr->prev;
    }
    /* Now in reverse order*/
    curr = queue->tail; /*reset curr to tail*/
    printf("\n---- TAIL TO HEAD -----\n");
    while(curr){
        printf("%s ", (char*) curr->data);
        curr = curr->next;
    }
}