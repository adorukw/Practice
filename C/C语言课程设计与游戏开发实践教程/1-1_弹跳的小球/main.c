#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int i, j;
    int y = 1;
    int x = 5;

    int top = 0;
    int bottom = 10;
    int left = 0;
    int right = 20;
    int velocity_x = 1;
    int velocity_y = 1;

    while (1) {
        x += velocity_x;
        y += velocity_y;
        system("clear");

        for (i = 0; i < y; i++) {
            printf("\n");
        }
        for (j = 0; j < x; j++) {
            printf(" ");
        }
        printf("o");
        printf("\n");

        if (x == left || x == right) {
            velocity_x = -velocity_x;
        }
        if (y == top || y == bottom) {
            velocity_y = -velocity_y;
        }
        sleep(1);
    }
    return 0;
}