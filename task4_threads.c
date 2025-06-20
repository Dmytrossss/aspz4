#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 4
#define ALLOC_COUNT 1000

void* thread_func(void* arg) {
    for(int i = 0; i < ALLOC_COUNT; i++) {
        void *ptr = malloc(1024);
        usleep(100);
        free(ptr);
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    
    for(int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }
    
    for(int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Усі потоки завершили роботу\n");
    return 0;
}
