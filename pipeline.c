#include "pipeline.h"

int client_fd;  // Client's file descriptor.
Deque* secondQ; // global queue.
Deque* thirdQ;  // global queue.

// Initializes pipe with active objects.
// return the generated pipe with initialized AOs.
_PIPE_PTR create_pipe(int fd, Deque* inputsQ){
    _PIPE_PTR pipe = (_PIPE_PTR) malloc(sizeof(_PIPE));
    client_fd = fd;

    /**********************************/
    /*         CREATE DEQUEUES        */
    /**********************************/
    secondQ = createQ();
    thirdQ = createQ();

    /**********************************/
    /*           CREATE AO            */
    /**********************************/
    AO_ptr firstAO = newAO(inputsQ, caesar_cipher, enQ2);
    AO_ptr secondAO = newAO(secondQ, alter_chars, enQ3);
    AO_ptr thirdAO = newAO(thirdQ, send_data, NULL);

    /**********************************/
    /*         SET PIPE'S AO          */
    /**********************************/
    pipe->first_AO = firstAO;
    pipe->second_AO = secondAO;
    pipe->third_AO = thirdAO;
    return pipe;
}

// Reads user's input from given socket of the server side.
void read_user_input(void* args){
    // Set given values to their original type
    ARGS *args1 = (ARGS*) args;
    int new_fd = args1->fd;
    Deque *deque = args1->deque;

    /**********************************/
    /*    READ STRINGS FROM SERVER    */
    /**********************************/
    for(;;){
        char buffer[MAX_DATA_SIZE];
        memset(buffer, 0, sizeof(buffer)); // clean buffer
        while (read(new_fd, buffer, sizeof(buffer)-1) > 0) {
            size_t buf_len = strlen(buffer);
            buffer[buf_len] = '\0';
            printf("block read: \n<%s>\n", buffer);
        }
        enQ(deque, buffer);
    }
}

// Method ciphers a given string input with the constant hist in header file.
// The cipher is a cesar cipher.
void caesar_cipher(void* user_in){
    char* user_in_str = (char*) user_in;
    size_t user_in_len = strlen(user_in_str);
    for (size_t i = 0; i < user_in_len; ++i) {
        char currChar = user_in_str[i];
        if (isupper(currChar)){
            char cipher_ch = (char) (currChar - 'A');
            cipher_ch += HIST;
            cipher_ch = (char) (cipher_ch % ALPHABETIC_AMOUNT);
            user_in_str[i] = (char) (cipher_ch + 'A');
        }
        else{ // Lowercase letter
            char cipher_ch = (char) (currChar - 'a');
            cipher_ch += HIST;
            cipher_ch = (char) (cipher_ch % ALPHABETIC_AMOUNT);
            user_in_str[i] = (char) (cipher_ch + 'a');
        }
    }
}

// Converts given string's lowercase chars to uppercase, and vice versa.
void alter_chars(void* user_in){
    char* user_in_str = (char*) user_in;
    size_t user_in_len = strlen(user_in_str);
    for (size_t i = 0; i < user_in_len; ++i) {
        char currChar = user_in_str[i];
        if (islower(currChar)){
            user_in_str[i] = (char) toupper(currChar);
        }
        else{
            user_in_str[i] = (char) tolower(currChar);
        }
    }
}

// Sends the data back to the user/client.
void send_data(void* user_in){
    char* str = (char*) user_in;
    send(client_fd, str, strlen(str), 0); // sends data back to client.
    printf("Data sent back to user: '%s'.\n", str);
}

// Enqueues to the second global queue.
void enQ2(void* data){
    enQ(secondQ, (char*)data);
}

// Enqueues to the third global queue.
void enQ3(void* data){
    enQ(thirdQ, (char*)data);
}

// Function destroys the pipeline.
void destroyPipe(_PIPE_PTR pipeline){
    destroyAO(pipeline->first_AO);
    destroyAO(pipeline->second_AO);
    destroyAO(pipeline->third_AO);
    free(pipeline);
}