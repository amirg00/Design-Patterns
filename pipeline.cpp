#include "pipeline.hpp"


_PIPE_PTR create_pipe(){
    return NULL;
}

// Reads user's input from given socket of the server side.
void read_user_input(){
    char buffer[MAX_DATA_SIZE];
    int sock, conn_res;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error in socket");
        exit(1);
    }
    printf("Server socket created successfully.\n");

    // creates the new connection
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = PORT;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    conn_res = connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (conn_res == -1) {
        perror("Error in socket");
        exit(1);
    }

    memset(buffer, 0, sizeof(buffer)); // clean buffer
    while ((buffer = read(sock, buffer, sizeof(buffer)-1)) > 0) {
        size_t buf_len = strlen(buffer);
        buffer[buf_len] = '\0';
        printf("block read: \n<%s>\n", buffer);
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