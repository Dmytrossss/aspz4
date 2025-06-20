#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = realloc(NULL, 1024);
    printf("1. realloc(NULL, 1024) повернув: %p\n", ptr);
    
    if(ptr == NULL) {
        fprintf(stderr, "Помилка: не вдалося виділити пам'ять\n");
        return 1;
    }
    
    void *new_ptr = realloc(ptr, 0);
    printf("2. realloc(ptr, 0) повернув: %p\n", new_ptr);
    
    if(new_ptr == NULL) {
        printf("3. Поведінка: оригінальний блок звільнено\n");
    } else {
        printf("3. Поведінка: повернуто новий вказівник на блок нульового розміру\n");
        printf("   Цей вказівник МОЖНА і ПОТРІБНО передавати у free()\n");
        free(new_ptr);
        printf("4. Новий блок успішно звільнено\n");
    }
    
    return 0;
}
