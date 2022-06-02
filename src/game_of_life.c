// Copyright semiotic, norreydi, singedto
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define HEIGHT 25
#define WIDTH 80

void draw(int universe[HEIGHT][WIDTH], int generation, double speed);
int number_of_neighbors(int universe[HEIGHT][WIDTH], int x, int y);
int next_generation(int universe[HEIGHT][WIDTH], int* generation);
void input(int universe[HEIGHT][WIDTH]);
void fill(int universe[HEIGHT][WIDTH]);
int input_key(double* speed);
int module(int a, int b);
char getch();

int main() {
    int universe[HEIGHT][WIDTH];
    int generation = 1;
    double speed = 1;

    fill(universe);
    input(universe);
    draw(universe, generation, speed);

    stdin = freopen("/dev/tty", "r", stdin);
    system("stty cbreak");

    while (input_key(&speed) && next_generation(universe, &generation) && \
           generation != 2147483647) {
        draw(universe, generation, speed);

        usleep(speed * 100000);
    }

    return 0;
}

void draw(int universe[HEIGHT][WIDTH], int generation, double speed) {
    printf("\e[H\e[2J\e[3J");
    printf("Generation: %d\n", generation);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            universe[i][j] ? printf("#") : printf(" ");
        printf("\n");
    }
    printf("Speed: %.1lfx\n", 1 - speed + 1);
}

int number_of_neighbors(int universe[HEIGHT][WIDTH], int x, int y) {
    int neighbours = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if ((universe[module(i, HEIGHT)][module(j, WIDTH)] == 1) && \
                (i != x || j != y))
                neighbours++;
        }
    }
    return neighbours;
}

int next_generation(int universe[HEIGHT][WIDTH], int* generation) {
    int next_universe[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            next_universe[i][j] = 0;
            int neighbours = number_of_neighbors(universe, i, j);

            if (universe[i][j] == 0 && neighbours == 3)
                    next_universe[i][j] = 1;

            if (universe[i][j] == 1) {
                if ((neighbours < 2) || (neighbours > 3))
                    next_universe[i][j] = 0;
                if ((neighbours == 3) || (neighbours == 2))
                    next_universe[i][j] = 1;
            }
        }
    }

    int flag = 0;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (next_universe[i][j] != universe[i][j]) {
                flag = 1;
                universe[i][j] = next_universe[i][j];
            }

    (*generation)++;

    return flag;
}

void fill(int universe[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            universe[i][j] = 0;
}

void input(int universe[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            scanf("%d", &universe[i][j]);
}

int input_key(double* speed) {
    int flag = 1;
    char c;
    c = getch();
    if (c == ' ') {
        c = '#';
        while (c != ' ')
            c = getch();
    } else if (c == 'z' && (1 - *speed + 1) > 0.1) {
        *speed += 0.1;
    } else if (c == 'x' && (1 - *speed + 1) < 1.9) {
        *speed -= 0.1;
    } else if (c == 'q') {
        flag = 0;
    }
    return flag;
}

int module(int a, int b) {
    return (b + (a % b)) % b;
}

char getch() {
    char c;
    int old = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, old | O_NONBLOCK);
    c = getchar();
    fcntl(0, F_SETFL, old);
    return c;
}
