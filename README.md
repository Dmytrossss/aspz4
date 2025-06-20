# Звіт виконання лабораторної роботи №4


**Виконав: Гнилицький Дмитро**

**Група: ТВ-33** 

**Варіант №3**

**ОС:** FreeBSD 13.4  

**Середовище:** VirtualBox з інтерфейсом MATE  

## Зміст
1. [Завдання 4.1: Максимальний розмір malloc()](#завдання-41-максимальний-розмір-malloc)
2. [Завдання 4.2: Від'ємний аргумент у malloc()](#завдання-42-відємний-аргумент-у-malloc)
3. [Завдання 4.3: malloc(0)](#завдання-43-malloc0)
4. [Завдання 4.4: Помилка у циклі](#завдання-44-помилка-у-циклі)
5. [Завдання 4.5: Помилка realloc()](#завдання-45-помилка-realloc)
6. [Завдання 4.6: Особливі випадки realloc()](#завдання-46-особливі-випадки-realloc)
7. [Завдання 4.7: reallocarray()](#завдання-47-reallocarray)
8. [Варіант 3: Багатопотоковий malloc()](#варіант-3-багатопотоковий-malloc)
9. [Загальні висновки](#загальні-висновки)

---

## Завдання 4.1: Максимальний розмір malloc()

```c
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
```

**Компіляція та запуск:**
```bash
clang -o task4_1 task4_1.c
./task4_1
```

**Вивід:**
![image](https://github.com/user-attachments/assets/7e955882-c107-4e8d-8a64-ce5050fd2623)


**Висновок:**  
Теоретично `malloc()` може виділити до 16 ЕБ (ексабайт) пам'яті (2^64 байт), проте на практиці це обмежено архітектурою процесора (сучасні CPU підтримують 48-бітну адресацію, що дає 256 ТБ) та налаштуваннями ядра ОС.

---

## Завдання 4.2: Від'ємний аргумент у malloc()

```c
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
```

**Компіляція та запуск:**
```bash
clang -o task4_2 task4_2.c
./task4_2
```

**Вивід:**
![image](https://github.com/user-attachments/assets/517dd233-d080-46f8-b4ea-dcd9f1615c66)


**Висновок:**  
Від'ємні аргументи небезпечні для `malloc()`, оскільки вони неявно перетворюються на великі додатні значення через беззнаковий тип `size_t`. Це може призвести до спроби виділення надмірно великого блоку пам'яті.

---

## Завдання 4.3: malloc(0)

```c
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
```

**Запуск з ltrace:**
```bash
clang -o task4_3 task4_3.c
ltrace ./task4_3 2>&1 | tee ltrace_output.txt
```

**Вивід програми:**
![image](https://github.com/user-attachments/assets/5b395160-17d0-46e1-83dd-2b943c7a2c12)


**Висновок:**  
`malloc(0)` у FreeBSD повертає спеціальний ненульовий вказівник, який можна безпечно передавати у `free()`. Це відповідає стандарту POSIX і запобігає витокам пам'яті.

---

## Завдання 4.4: Помилка у циклі

**Код з помилкою:**
```c
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
```

**Виправлений код:**
```c
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
        
        counter++;
    }
    
    free(ptr);
    return 0;
}
```

**Результат виконання:**
Помилковий та правильний код

![image](https://github.com/user-attachments/assets/b046a861-1ca9-4b96-9119-4f2ece14e397)


**Висновок:**  
Звільнення пам'яті всередині циклу призводить до використання вже звільненої пам'яті (висівшого вказівника). Правильний підхід - виділяти пам'ять перед циклом і звільняти після завершення роботи з нею.

---

## Завдання 4.5: Помилка realloc()

```c
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
```

**Вивід:**
![image](https://github.com/user-attachments/assets/236a130c-dba6-42a9-b3d7-da1899969115)


**Висновок:**  
При невдалому `realloc()` оригінальний блок пам'яті залишається недоторканим, що дозволяє продовжити роботу з ним або коректно звільнити.

---

## Завдання 4.6: Особливі випадки realloc()

```c
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
```

**Вивід:**
![image](https://github.com/user-attachments/assets/624c55f1-51db-4fd8-84ab-7cac70573b54)


**Висновок:**  
У FreeBSD `realloc(ptr, 0)` повертає новий вказівник на блок нульового розміру замість NULL, що вимагає додаткового звільнення пам'яті.

---

## Завдання 4.7: reallocarray()

**Оригінальний код:**
```c
#include <stdlib.h>

struct sbar {
    int value;
};

int main() {
    struct sbar *ptr, *newptr;
    ptr = calloc(1000, sizeof(struct sbar));
    newptr = realloc(ptr, 500 * sizeof(struct sbar));
    free(newptr);
    return 0;
}
```

**Код з reallocarray:**
```c
#include <stdlib.h>

struct sbar {
    int value;
};

int main() {
    struct sbar *ptr, *newptr;
    ptr = calloc(1000, sizeof(struct sbar));
    newptr = reallocarray(ptr, 500, sizeof(struct sbar));
    free(newptr);
    return 0;
}
```

**Порівняння з ltrace:**
```bash
ltrace ./task4_7_orig 2> trace_orig.txt
ltrace ./task4_7_realloc 2> trace_realloc.txt
```

**Вивід `trace_orig.txt`:**

![image](https://github.com/user-attachments/assets/a906d6b3-a8fd-4790-ac13-1615cfabf118)


**Вивід `trace_realloc.txt`:**

![image](https://github.com/user-attachments/assets/82703d76-e9e0-4165-b6a9-c81bace85297)


**Висновок:**  
`reallocarray()` є безпечнішою альтернативою `realloc()` при роботі з множенням розмірів, оскільки запобігає переповненням цілих чисел.

---

## Варіант 3: Багатопотоковий malloc()

```c
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
```

**Компіляція та запуск:**
```bash
clang -pthread -o task4_threads task4_threads.c
./task4_threads
```

**Вивід:**
![image](https://github.com/user-attachments/assets/d471ddc5-1d87-4193-87e9-2dccd44a4997)


**Висновок:**  
Алокатор пам'яті jemalloc у FreeBSD ефективно забезпечує потокобезпеку, дозволяючи паралельне виконання операцій з пам'яттю без конфліктів.

---

## Загальні висновки

1. **Теоретичні та практичні обмеження**: Максимальний розмір пам'яті для `malloc()` обмежений архітектурою системи, а не лише розміром `size_t`.

2. **Типова безпека**: Аргументи функцій роботи з пам'яттю завжди інтерпретуються як беззнакові значення, що вимагає обережності при передачі змінних.

3. **Спеціальні випадки**:
   - `malloc(0)` повертає дійсний вказівник для `free()`
   - `realloc(ptr, 0)` у FreeBSD вимагає додаткового звільнення
   - Невдалий `realloc()` не впливає на оригінальний блок

4. **Безпечні альтернативи**: `reallocarray()` є кращою практикою для операцій з множенням розмірів.

5. **Потокобезпека**: FreeBSD ефективно обробляє паралельні запити на виділення пам'яті завдяки jemalloc.
