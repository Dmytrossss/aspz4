#include <stdio.h>
#include <stdlib.h>

int main() {
    void* ptr = malloc(0);
    
    printf("Результат malloc(0): %p\n", ptr);
    
    if(ptr != NULL) {
        printf("Звільнення ненульового вказівника\n");
        free(ptr);
        printf("Пам'ять успішно звільнена\n");
    } else {
        printf("malloc повернув NULL\n");
    }
    
    return 0;
}
