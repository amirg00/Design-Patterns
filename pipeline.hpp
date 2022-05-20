#include "Object_active.hpp"
#include <ctype.h>

#define ALPHABETIC_AMOUNT 26        /*Alphabetic chars amount*/
#define HIST 1                      /*Hist amount for the cipher: the amount of the shift*/
#define PORT "3490"                 /*The port which first AO will be connecting to*/
#define SERVER_IP "127.0.0.1"       /*The server ip is the localhost*/
#define MAX_DATA_SIZE 4096          /*Max number of bytes we can get at once - buffer size*/


typedef struct pipeline{
    AO_ptr first_AO;        /*Reads an input in socket which is connected to server*/
    AO_ptr second_AO;       /*Activate 'Caesar Cipher' in hist=1, for each input user enters*/
    AO_ptr third_AO;        /*Convert lowercase chars to uppercase, and vice versa*/
    AO_ptr forth_AO;        /*Returns an answer to the user*/
}_PIPE, *_PIPE_PTR;

_PIPE_PTR create_pipe();
void read_user_input();
void caesar_cipher(char* user_in);
void alter_chars(char* user_in);
