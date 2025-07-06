#include <stdio.h>

int fPScore = 0;
int sPScore = 0;

void cross_and_toe_display(int flag);
int f_board_key(char key, int fRStart, int fREnd, int min_y, int max_y);
int second_board_key(char key, int sRStart, int sREnd, int min_y, int max_y);
int check_winner(int pointX, int max_x);
int handle_collisions(int pointY, int pointSpeedY, int max_y);
int handle_racket_bounce(int pointX, int pointY, int fRStart, int fREnd,
                         int sRStart, int sREnd, int pointSpeedX,
                         int firstRacketX, int secondRacketX);

int main(void) {
    while (1) {
        int flag = 1;
        cross_and_toe_display(flag);
    }
}

void cross_and_toe_display(int flag) {
    int fRStart = 5, fREnd = 7, firstRacketX = 3;
    int sRStart = 17, sREnd = 19, secondRacketX = 76;
    int pointX = 39, pointY = 12, pointSpeedX = 1, pointSpeedY = 1;
    int max_y = 25, max_x = 80, min_y = 1;
    char vertical = '|', horizontal = '-', space = ' ', point = '*';

    while (flag) {
        printf("\033[0d\033[2J");

        for (int i = 0; i < max_y; i++) {
            for (int j = 0; j < max_x; j++) {
                if (j == 0 || j == max_x - 1)
                    printf("%c", vertical);
                else if (i == 0 || i == max_y - 1)
                    printf("%c", horizontal);
                else if (i == pointY && j == pointX)
                    printf("%c", point);
                else if ((i >= fRStart && i <= fREnd && j == firstRacketX) ||
                         (i >= sRStart && i <= sREnd && j == secondRacketX))
                    printf("%c", vertical);
                else
                    printf("%c", space);
            }
            printf("\n");
        }

        printf("Points First's player: %d\n", fPScore);
        printf("Points Second's player: %d\n", sPScore);

        pointSpeedY = handle_collisions(pointY, pointSpeedY, max_y);

        if (check_winner(pointX, max_x)) {
            flag = 0;
            break;
        }

        pointSpeedX = handle_racket_bounce(pointX, pointY, fRStart, fREnd,
                                           sRStart, sREnd, pointSpeedX,
                                           firstRacketX, secondRacketX);

        char key = getchar();

        int move = f_board_key(key, fRStart, fREnd, min_y, max_y);
        if (move == 1)
            fRStart++, fREnd++;
        else if (move == -1)
            fRStart--, fREnd--;

        move = second_board_key(key, sRStart, sREnd, min_y, max_y);
        if (move == 1)
            sRStart++, sREnd++;
        else if (move == -1)
            sRStart--, sREnd--;

        if (key == ' ') {
            pointX += pointSpeedX;
            pointY += pointSpeedY;
        }
    }
}

int f_board_key(char key, int fRStart, int fREnd, int min_y, int max_y) {
    if (key == 'a' && fRStart > min_y)
        return -1;
    if (key == 'z' && fREnd < max_y - 2)
        return 1;
    return 0;
}

int second_board_key(char key, int sRStart, int sREnd, int min_y, int max_y) {
    if (key == 'k' && sRStart > min_y)
        return -1;
    if (key == 'm' && sREnd < max_y - 2)
        return 1;
    return 0;
}

int check_winner(int pointX, int max_x) {
    if (pointX == max_x - 1) {
        fPScore++;
        if (fPScore == 21) {
            printf("Win First player!\n");
        }
        return 1;
    }
    if (pointX == 0) {
        sPScore++;
        if (sPScore == 21) {
            printf("Win Second player!\n");
        }
        return 1;
    }
    return 0;
}

int handle_collisions(int pointY, int pointSpeedY, int max_y) {
    if (pointY == max_y - 2)
        return -1;
    else if (pointY == 1)
        return 1;
    return pointSpeedY;
}

int handle_racket_bounce(int pointX, int pointY, int fRStart, int fREnd,
                         int sRStart, int sREnd, int pointSpeedX,
                         int firstRacketX, int secondRacketX) {
    if (sRStart <= pointY && pointY <= sREnd && pointX == secondRacketX - 1)
        return -1;
    else if (fRStart <= pointY && pointY <= fREnd && pointX == firstRacketX + 1)
        return 1;
    return pointSpeedX;
}