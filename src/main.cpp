#include "stdio.h"

int main() {
    if (!init()) {
        fprintf(stderr, "ERROR: Failed to initialise.\n");
        return -1;
    }

    return 0;
}

int init() {
    

    return 1;
}
