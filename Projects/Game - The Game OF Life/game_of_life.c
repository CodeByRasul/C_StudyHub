#include <ncurses.h>
#include <stdio.h>

int load_file(const char* name, int field[25][80]) {
    FILE* f = fopen(name, "r");
    int i, j, c;
    if (!f) {
        return 0;
    }
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 80; j++) {
            field[i][j] = 0;
        }
    }
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 80; j++) {
            c = fgetc(f);
            if (c == EOF) {
                break;
            }
            if (c == '*') {
                field[i][j] = 1;
            } else if (c == '\n') {
                j--;
            }
        }
        while ((c = fgetc(f)) != '\n' && c != EOF) {
        }
    }
    fclose(f);
    return 1;
}

void draw(const int field[25][80], int speed) {
    int i, j;
    clear();
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 80; j++) {
            if (field[i][j]) {
                mvprintw(i, j, "*");
            }
        }
    }
    mvprintw(25 + 1, 0, "Speed: %d | A/Z - speed, Space - exit", speed);
    refresh();
}

int count_neighbors(const int field[25][80], int r, int c) {
    int count = 0, i, j;
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (i != 0 || j != 0) {
                count += field[(r + i + 25) % 25][(c + j + 80) % 80];
            }
        }
    }
    return count;
}

void update_field(int field[25][80], int new_field[25][80]) {
    int i, j, n;
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 80; j++) {
            n = count_neighbors(field, i, j);
            if (field[i][j] == 1) {
                if (n == 2 || n == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            } else {
                if (n == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            }
        }
    }
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 80; j++) {
            field[i][j] = new_field[i][j];
        }
    }
}

int get_delay(int speed) {
    const int delays[10] = {2000, 1500, 1000, 750, 500, 350, 250, 150, 100, 50};
    return delays[speed - 1];
}

int check_input(int* speed) {
    int key = getch();
    if (key == ' ') {
        return 0;
    }
    if ((key == 'A' || key == 'a') && *speed < 10) {
        *speed = *speed + 1;
    }
    if ((key == 'Z' || key == 'z') && *speed > 1) {
        *speed = *speed - 1;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int field[25][80], new_field[25][80];
    int speed = 3;
    int running = 1;
    int i;
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    if (load_file(argv[1], field) == 0) {
        printf("Error loading file\n");
        return 1;
    }
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
    while (running) {
        draw(field, speed);
        if (check_input(&speed) == 0) {
            running = 0;
        } else {
            update_field(field, new_field);
            for (i = 0; i < get_delay(speed) / 10 && running; i++) {
                napms(10);
                if (check_input(&speed) == 0) {
                    running = 0;
                }
            }
        }
    }
    endwin();
    return 0;
}