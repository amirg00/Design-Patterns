#include "pthread.h"
#include <poll.h>
#include <unistd.h>

#define F_NUM 50
#define FD_NUM 100
#define MAX_DATA 1024

typedef struct Reactor{
    void* (*func_ptr[F_NUM])(void*);
    struct pollfd fds[FD_NUM];
    int fd_size;
    pthread_t private_thread;
}REACTOR, *REACTOR_PTR;

void* newReactor();
void InstallHandler(struct Reactor* reactor_ptr, void* (*func_ptr)(void*), int fd);
void RemoveHandler(struct Reactor* reactor_ptr);
void listen_to_fds(struct Reactor* reactor_ptr);