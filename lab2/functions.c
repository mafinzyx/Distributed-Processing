#include "types.h"
#include <stdio.h>

int add(data_t x, data_t y) {
    return(x.a + x.b + y.a + y.b);
}

int subtract(data_t x, data_t y) {
    return (x.a + x.b) - (y.a + y.b);
}

int multiply(data_t x, data_t y) {
    return (x.a + x.b) * (y.a + y.b);
}

float divide(data_t x, data_t y) {
    if (y.a + y.b == 0) {
        printf("Error: division by zero!\n");
        return 0.0;
    }
    return (float)(x.a + x.b) / (y.a + y.b);
}
