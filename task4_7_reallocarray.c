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
