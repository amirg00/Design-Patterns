/*
** pollserver.c -- a cheezy multiperson chat server
 * credit to: https://beej.us/guide/bgnet/examples/pollserver.c
*/
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include "reactor.h"

#define PORT "9034"   // Port we're listening on

REACTOR_PTR reactorPtr = newReactor(); // global reactor


// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void) {
    int listener;     // Listening socket descriptor
    int yes = 1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }
        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    freeaddrinfo(ai); // All done with this

    // Listen: up to 10 client in queue
    if (listen(listener, 10) == -1) {
        return -1;
    }
    return listener;
}

// Function handles new connection the listener got.
// Function gets the listener's fd.
void handle_new_connection(void* fd) {
    int newfd;
    int *listener_fd = (int*) fd;
    int listener = *listener_fd;

    addrlen = sizeof remoteaddr; // address
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];

    newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);
    if (newfd == -1) {
        perror("accept\n");
        return;
    }
    // Install receive function for regular clients
    // since listener got a connection from regular client.
    InstallHandler(reactorPtr, handle_recv, newfd);
    printf("pollserver: new connection from %s on socket %d\n",
           inet_ntop(remoteaddr.ss_family,
                     get_in_addr((struct sockaddr *) &remoteaddr),
                     remoteIP, INET6_ADDRSTRLEN),
           newfd);

}

// Handles receive data for regular clients.
// Function gets the file descriptor of the regular client.
void handle_recv(void* fd){
    int *regular_fd = (int*) fd;
    int regular_client = *regular_fd;
    char buf[256];    // Buffer for client data

    // Receive data from the regular client.
    // This is a broadcast message.
    int nbytes = recv(regular_client, buf, sizeof buf, 0);

    if (nbytes <= 0) {
        // Got error or connection closed by client
        if (nbytes == 0) {
            // Connection closed
            printf("pollserver: socket %d hung up\n", regular_client);
        } else {
            perror("recv");
        }
        close(regular_client); // Bye!
        RemoveHandler(reactorPtr, regular_client);
    }else {
        // We got some good data from a client
        for (int j = 0; j < FD_NUM; j++) {
            // Send to everyone! (broadcast message)
            int dest_fd = reactorPtr->fds[j].fd;
            if(dest_fd == -1) {continue;}

            // Except the listener and ourselves
            if (reactorPtr->func_ptr[j] != handle_new_connection && dest_fd != regular_client) {
                if (send(dest_fd, buf, nbytes, 0) == -1) {
                    perror("send");
                }
            }
        }
    }
}

int main() {
    int listener; // Listening socket descriptor

    // Set up and get a listening socket
    listener = get_listener_socket();
    if (listener == -1) {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }
    // install handler for the listener
    // to handle new connections.
    InstallHandler(reactorPtr, handle_new_connection, listener);
    pthread_join(reactorPtr->private_thread, NULL); // wait for thread to terminate.
    close(listener);
    return 0;
}