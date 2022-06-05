#include "Object_active.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <ctype.h>

#define ALPHABETIC_AMOUNT 26        /*Alphabetic chars amount*/
#define HIST 1                      /*Hist amount for the cipher: the amount of the shift*/
#define PORT 3490                   /*The port which first AO will be connecting to*/
#define SERVER_IP "127.0.0.1"       /*The server ip is the localhost*/
#define MAX_DATA_SIZE 4096          /*Max number of bytes we can get at once - buffer size*/


typedef struct pipeline{
    AO_ptr first_AO;        /*Activate 'Caesar Cipher' in hist=1, for each input user enters*/
    AO_ptr second_AO;       /*Convert lowercase chars to uppercase, and vice versa*/
    AO_ptr third_AO;        /*Returns an answer to the user*/
}_PIPE, *_PIPE_PTR;

_PIPE_PTR create_pipe();

[[noreturn]] void read_user_input(void* sock_fd, void* queue);
void caesar_cipher(void* user_in);
void alter_chars(void* user_in);
void send_data(void* user_in);
void enQ2(void* data);
void enQ3(void* data);
void destroyPipe(_PIPE_PTR pipeline);
