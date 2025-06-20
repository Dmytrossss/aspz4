#include <stdio.h>
#include <stdlib.h>

int main() {
    const size_t reasonable_size = 1024;
    const size_t huge_size = (size_t)-1;
    
    void *ptr = malloc(reasonable_size);
    if(!ptr) {
        perror("Початкове виділення невдале");
        return 1;
    }
    
    printf("Початковий блок: %p\n", ptr);
    
    void *new_ptr = realloc(ptr, huge_size);
    if(!new_ptr) {
        perror("realloc невдалий");
        printf("Але оригінальний блок ще дійсний: %p\n", ptr);
        free(ptr);
    } else {
        printf("Новий блок: %p\n", new_ptr);
        free(new_ptr);
    }
    
    return 0;
}
