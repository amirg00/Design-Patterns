#include "reactor.hpp"

void* newReactor(){
    REACTOR_PTR reactor = (REACTOR) malloc(sizeof(REACTOR));
    reactor.fd_size = 0;
    return reactor;
}
void InstallHandler(REACTOR_PTR reactor_ptr, void* (*func_ptr)(void*), int fd){
    if(!reactor_ptr->fd_size){ // An empty reactor
        /*Fill reactor's fields*/
        reactor_ptr->fds[0].events = POLLIN; // Report ready to read on incoming connection
        reactor_ptr->fds[0].fd = fd;
        reactor_ptr->func_ptr[0] = func_ptr;
        reactor_ptr->fd_size++;

        // Create rector's thread id
        if (pthread_create(&reactor->private_thread, NULL, listen_to_fds, reactor) != 0) {
            printf("Unable to create thread\n");
        }
        return;
    }
    // Add a new fd handler in the end of the fds array.
    int curr_fd_size = reactor_ptr->fd_size;
    reactor_ptr->fds[curr_fd_size].events = POLLIN; // Report ready to read on incoming connection
    reactor_ptr->fds[curr_fd_size] = fd;
    reactor_ptr->func_ptr[curr_fd_size] = func_ptr;
    reactor_ptr->fd_size++;
}

void RemoveHandler(REACTOR_PTR reactor_ptr, int fd){
    for (int i = 0; i < reactor_ptr->fd_size; ++i) {
        if(reactor_ptr->fds[i].fd == fd){
            reactor_ptr->fds[i].fd = -1;
            reactor_ptr->fds[i].events = 0;
            reactor_ptr->func_ptr[reactor_ptr->fd_size - 1] = NULL; // Remove associated function
            reactor_ptr->fd_size--;
            return;
        }
    }

}

void listen_to_fds(REACTOR_PTR reactor_ptr){
    for(;;){
        int poll_counter = poll(reactor_ptr->fds, FD_NUM, -1);
        if (poll_count == -1) {
            perror("Poll error!\n");
            exit(1);
        }
        // Goes over all fds check who changed,
        // then sends read data from the socket.
        for (int fd = 0; fd < reactor_ptr->fd_size; i++){
            if(reactor_ptr->fds[fd].revents && POLLIN){
                // read input and send it to the corresponding function.
                char buffer[MAX_DATA];
                int rc = read(sock, buffer, sizeof(buffer));
                if (rc <= 0) perror("Error reading!\n");
                reactor_ptr->func_ptr[fd](buffer);
            }
        }

    }
}


