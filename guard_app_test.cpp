#include "guard.hpp"
#include "iostream"
#include <unistd.h>
#include "pthread.h"
using namespace std;


/**
 * Brief: this application test increases a global int variable.
 *        In order to see the lock effect, we sleep the thread,
 *        which locked the function, to see that didn't increase
 *        the variable at the same time.
 */

pthread_mutex_t lock; // lock for the guard.
int *global_num = (int*) malloc(sizeof(int)); // global int variable, which will be increased by threads.

// Increases global int by 1.
void* update_global_var(void*){
    guard guard{lock};
    (*global_num)++; //increase global by one.
    cout << "Variable's Current Value: " << *global_num << endl;
    sleep(10);
}

int main(){
    *global_num = 0; // Initial variable's value is set to zero.

    // Initialize the lock.
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }
    pthread_t pthread_1, pthread_2;
    pthread_create(&pthread_1, NULL,  update_global_var, NULL);
    pthread_create(&pthread_2, NULL,  update_global_var, NULL);
    pthread_join(pthread_1, NULL);
    pthread_join(pthread_2, NULL);
    free(global_num);
}