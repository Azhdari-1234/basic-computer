#include <stdio.h>
#include <windows.h>

#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define MAGENTA FOREGROUND_RED | FOREGROUND_BLUE
#define CYAN FOREGROUND_GREEN | FOREGROUND_BLUE
#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int x = 8;
int y = 8;
char main_map[8][8];

void printMap() {
    int x_index = x - 1;
    int y_index = y - 1;

    set_color(CYAN);
    printf("  %c", 201);
    for(int i = 0; i < x_index; i++) printf("%c%c%c%c", 205, 205, 205, 209);
    printf("%c%c%c%c\n", 205, 205, 205, 187);

    for(int i = y_index; i >= 0; i--) {
        set_color(CYAN);
        printf("%2d%c", i+1, 186);
        for(int j = 0; j <= x_index-1; j++) printf(" %c %c", main_map[j][i], 179);
        printf(" %c %c", main_map[x_index][i], 186);
        printf("\n");
        if(i > 0) {
            set_color(CYAN);
            printf("  %c", 199);
            for(int j = 0; j < x_index; j++) printf("%c%c%c%c", 196, 196, 196, 197);
            printf("%c%c%c%c\n", 196, 196, 196, 182);
        }
    }

    set_color(CYAN);
    printf("  %c", 200);
    for(int i = 0; i < x_index; i++) printf("%c%c%c%c", 205, 205, 205, 207);
    printf("%c%c%c%c\n   ", 205, 205, 205, 188);

    set_color(CYAN);
    for(int i = 0; i <= x_index; i++) printf(" %d  ", i+1);
    set_color(WHITE);
    printf("\n");
}

int main() {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            main_map[i][j] = ' '; // Replace with actual values
        }
    }

    printMap();
    

    return 0;
}
