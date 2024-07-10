#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define MAGENTA FOREGROUND_RED | FOREGROUND_BLUE
#define CYAN FOREGROUND_GREEN | FOREGROUND_BLUE
#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

void set_color(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int x = 8;
int y = 8;
int main_map[8][8];

char player1[50], player2[50];
int score1 = 0, score2 = 0;
void printMap();

void placeBombs(int num_bombs)
{
    srand(time(NULL));
    int placed_bombs = 0;
    while (placed_bombs < num_bombs)
    {
        int bomb_x = rand() % x;
        int bomb_y = rand() % y;

        if (main_map[bomb_x][bomb_y] == 0)
        {
            main_map[bomb_x][bomb_y] = 1;
            placed_bombs++;
        }
    }
}
int countBombs(int cell_x, int cell_y)
{
    int count = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int nx = cell_x + dx;
            int ny = cell_y + dy;
            if (nx >= 0 && nx < x && ny >= 0 && ny < y && main_map[nx][ny] == 1)
            {
                count++;
            }
        }
    }
    return count;
}
void revealAdjacent(int cell_x, int cell_y)
{
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int nx = cell_x + dx;
            int ny = cell_y + dy;
            if (nx >= 0 && nx < x && ny >= 0 && ny < y && main_map[nx][ny] == 0)
            {
                main_map[nx][ny] = 4;
            }
        }
    }
}
void playerTurn(char player[50], int playerNumber)
{
    printf("\n");
    printMap();
    printf("%s's turn. Enter your choice (x y): ", player);
    int choice_x, choice_y;
    scanf("%d %d", &choice_x, &choice_y);
    choice_x--;
    choice_y--;

    if (choice_x < 0 || choice_x >= x || choice_y < 0 || choice_y >= y)
    {
        printf("Invalid choice. Please choose a house within the map.\n");
        playerTurn(player, playerNumber);
    }
    else if (main_map[choice_x][choice_y] > 1)
    {
        printf("This house has already been selected. Please choose a different house.\n");
        playerTurn(player, playerNumber);
    }
    else if (main_map[choice_x][choice_y] == 1)
    {
        printf("You found a bomb!\n");
        main_map[choice_x][choice_y] = playerNumber;
        if (playerNumber == 2)
        {
            score1 = score1 + 1;
        }
        if (playerNumber == 3)
        {
            score2 = score2 + 1;
        }
        if (score1 == 3 || score2 == 3)
        {
            printf("\n");
            return;
        }
        playerTurn(player, playerNumber);
    }
    else
    {
        printf("No bomb here.\n");
        if (countBombs(choice_x, choice_y) == 0)
        {
            revealAdjacent(choice_x, choice_y);
        }
        else
        {
            main_map[choice_x][choice_y] = 4;
        }
    }
}
char getCellValue(int cell_x, int cell_y)
{
    switch (main_map[cell_x][cell_y])
    {
    case 0:
    case 1:
        set_color(WHITE);
        return ' ';
    case 2:
        set_color(RED);
        return '@';
    case 3:
        set_color(CYAN);
        return '#';
    case 4:
        return '0' + countBombs(cell_x, cell_y);
    default:
        return '?';
    }
}

void printMap()
{
    int x_index = x - 1;
    int y_index = y - 1;

    set_color(WHITE);
    printf("  %c", 201);
    for (int i = 0; i < x_index; i++)
        printf("%c%c%c%c", 205, 205, 205, 209);

    printf("%c%c%c%c\n", 205, 205, 205, 187);

    for (int i = y_index; i >= 0; i--)
    {
        printf("%2d%c", i + 1, 186);
        for (int j = 0; j < x_index; j++)
        {
            printf(" %c", getCellValue(j, i));
            set_color(WHITE);
            printf(" %c", 179);
        }
        printf(" %c %c", getCellValue(x_index, i), 186);
        printf("\n");
        
        if (i > 0)
        {
            printf("  %c", 199);
            for (int j = 0; j < x_index; j++)
                printf("%c%c%c%c", 196, 196, 196, 197);
            printf("%c%c%c%c\n", 196, 196, 196, 182);
        }
    }

    printf("  %c", 200);
    for (int i = 0; i < x_index; i++)
        printf("%c%c%c%c", 205, 205, 205, 207);
    printf("%c%c%c%c\n   ", 205, 205, 205, 188);

    for (int i = 0; i <= x_index; i++)
        printf(" %d  ", i + 1);
    printf("\n");
}
// Function to find the level of a player
int findPlayerLevel(const char *playerName, FILE *file)
{
    char name[50];
    int level;
    int found = 0; // Flag to track if player is found
    fseek(file, 0, SEEK_SET);

    while (fscanf(file, "%s %d", name, &level) == 2)
    {
        if (strcmp(name, playerName) == 0)
        {
            found = 1;
            return level;
        }
    }

    // If player not found, add player with level 0
    if (!found)
    {
        fprintf(file, "%s 0\n", playerName);
        return 0;
    }
    return -1;
}
void levelUpPlayer(const char *playerName, FILE *file)
{
    char name[50];
    int level;
    long int pos; // To store the file position
    fseek(file, 0, SEEK_SET);

    while (fscanf(file, "%s %d", name, &level) == 2)
    {
        if (strcmp(name, playerName) == 0)
        {
            // Found the player, update the level
            pos = ftell(file);                             // Store the current file position
            fseek(file, pos - strlen(name) - 2, SEEK_SET); // Move back to the beginning of the line
            fprintf(file, "%s %d\n", name, level + 1);     // Update the level
            break;
        }
    }
}


int main()
{
    printf("Enter the name of the first player: ");
    scanf("%s", player1);
    printf("Enter the name of the second player: ");
    scanf("%s", player2);

    FILE* file = fopen("levels.txt", "r+");
    if (file != NULL)
    {
        char line[100];
        while (fgets(line, sizeof(line), file))
        {
            // printf("%s", line);
        }
    }
    else
        printf("Please first create level.txt\n");
        
    printf("\nPlayer1: %s     Level: %d\n", player1, findPlayerLevel(player1, file));
    printf("Player2: %s     Level: %d\n", player2, findPlayerLevel(player2, file));
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            main_map[i][j] = 0;
        }
    }
    int num_bombs = 15;
    placeBombs(num_bombs);

    while (1)
    {
        playerTurn(player1, 2);
        if (score1 >= 3)
        {
            printf("%s found %d bombs, %s found %d bombs.\n", player1, score1, player2, score2);
            printf("%s in winner ! ", player1);
            levelUpPlayer(player1, file);
            break;
        }
        playerTurn(player2, 3);
        if (score2 >= 3)
        {
            printf("%s found %d bombs, %s found %d bombs.\n", player1, score1, player2, score2);
            printf("%s in winner ! ", player2);
            levelUpPlayer(player2, file);
            break;
        }
    }
    fclose(file);

    return 0;
}
