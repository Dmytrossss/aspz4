#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = NULL;
    int counter = 0;
    
    while(counter < 3) {
        if(!ptr) {
            ptr = malloc(256);
            printf("Виділено пам'ять: %p\n", ptr);
        }
        
        int *data = (int*)ptr;
        *data = counter;
        printf("Значення: %d\n", *data);
        
        free(ptr);  // Критична помилка!
        counter++;
    }
    
    return 0;
}
