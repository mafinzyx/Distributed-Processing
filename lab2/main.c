#include "types.h"
#include "functions.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

    data_t x, y;

    x.a = 10;
    x.b = 5;
    y.a = 3;
    y.b = 2;

    int choice;
    printf("Choice operation:\n");
    printf("1. Addition\n");
    printf("2. Subtract\n");
    printf("3. Multiplication\n");
    printf("4. Divided\n");
    printf("Print your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Answer of Addition: %d\n", add(x, y));
            break;
        case 2:
            printf("Answer of Subtract: %d\n", subtract(x, y));
            break;
        case 3:
            printf("Answer of Multiplication: %d\n", multiply(x, y));
            break;
        case 4:
            printf("Answer of Divided: %.2f\n", divide(x, y));
            break;
        default:
            printf("Incorrect number!\n");
    }

    return 0;
}
