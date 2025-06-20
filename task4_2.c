#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main() {
    int xa = INT_MAX;
    int xb = 2;
    int num = xa * xb;  // Навмисне переповнення
    
    printf("Результат множення: %d\n", num);
    printf("Як size_t: %zu\n", (size_t)num);
    
    void* ptr = malloc(num);
    if(ptr == NULL) {
        perror("malloc повернув NULL");
    } else {
        printf("Виділено %d байт\n", num);
        free(ptr);
    }
    
    return 0;
}
