#include "pipeline.hpp"


_PIPE_PTR create_pipe(){
    _PIPE_PTR pipe = (_PIPE_PTR) malloc(sizeof(_PIPE));
    return pipe;
}

// Reads user's input from given socket of the server side.
void read_user_input(){
    AO_ptr in_ptr;
    char buffer[MAX_DATA_SIZE];
    int sock, conn_res;
    struct sockaddr_in server_addr;

    /**********************************/
    /*         CREATE DEQUE           */
    /**********************************/
    in_ptr = (AO_ptr) malloc(sizeof(AO));
    if (!in_ptr){return;}

    /**********************************/
    /*         SET AO FIELDS          */
    /**********************************/
    in_ptr->queue = createQ();


    /**********************************/
    /*         CREATE SOCKET          */
    /**********************************/
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error in socket");
        exit(1);
    }
    printf("Server socket created successfully.\n");

    /**********************************/
    /*     CREATES NEW CONNECTION     */
    /**********************************/
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = PORT;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    /**********************************/
    /*       CONNECTS TO SERVER       */
    /**********************************/
    conn_res = connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (conn_res == -1) {
        perror("Error in socket");
        exit(1);
    }

    /**********************************/
    /*    READ STRINGS FROM SERVER    */
    /**********************************/
    for(;;){
        memset(buffer, 0, sizeof(buffer)); // clean buffer
        while (read(sock, buffer, sizeof(buffer)-1) > 0) {
            size_t buf_len = strlen(buffer);
            buffer[buf_len] = '\0';
            printf("block read: \n<%s>\n", buffer);
        }
        enQ(in_ptr->queue, buffer);
    }
}

// Method ciphers a given string input with the constant hist in header file.
// The cipher is a cesar cipher.
void caesar_cipher(char* user_in){
    size_t user_in_len = strlen(user_in);
    for (size_t i = 0; i < user_in_len; ++i) {
        char currChar = user_in[i];
        if (isupper(currChar)){
            char cipher_ch = (char) (currChar - 'A');
            cipher_ch += HIST;
            cipher_ch = (char) (cipher_ch % ALPHABETIC_AMOUNT);
            user_in[i] = (char) (cipher_ch + 'A');
        }
        else{ // Lowercase letter
            char cipher_ch = (char) (currChar - 'a');
            cipher_ch += HIST;
            cipher_ch = (char) (cipher_ch % ALPHABETIC_AMOUNT);
            user_in[i] = (char) (cipher_ch + 'a');
        }
    }
}

// Converts given string's lowercase chars to uppercase, and vice versa.
void alter_chars(char* user_in){
    size_t user_in_len = strlen(user_in);
    for (size_t i = 0; i < user_in_len; ++i) {
        char currChar = user_in[i];
        if (islower(currChar)){
            user_in[i] = (char) toupper(currChar);
        }
        else{
            user_in[i] = (char) tolower(currChar);
        }
    }
}

// Function destroys the pipeline.
void destroyPipe(_PIPE_PTR pipeline){
    destroyAO(pipeline->first_AO);
    destroyAO(pipeline->second_AO);
    destroyAO(pipeline->third_AO);
    destroyAO(pipeline->forth_AO);
    free(pipeline);
}