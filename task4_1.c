#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    printf("Розмір size_t: %zu байт\n", sizeof(size_t));
    printf("Максимальне значення size_t: %zu\n", SIZE_MAX);
    
    void* ptr = malloc(SIZE_MAX);
    if(ptr == NULL) {
        perror("Помилка виділення пам'яті");
        printf("Фактичний максимум менший за теоретичний\n");
    } else {
        printf("Успішно виділено %zu байт\n", SIZE_MAX);
        free(ptr);
    }
    
    return 0;
}
