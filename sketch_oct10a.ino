#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <LiquidCrystal.h>

#define board_height 2
#define board_width 16
#define garbage_value 10
#define game_turns 1000
#define garbage_interval 10
#define garbage_units 2

LiquidCrystal lcd(12, 7, 11, 5, 4, 3, 2);

char** board;
int robotx, roboty, score;
int dirx[4] = {-1, 0, 1, 0};
int diry[4] = {0, 1, 0, -1};

void init_board();

void del_board();

char** get_board();

void print_board();

void put_garbage(int, int);

void move_robot(int);

void load_garbage();

int fdm(char**);

int my_rand(int);

void setup() {
  lcd.begin(16,2);
}

void loop() {
    srand(time(NULL));
    init_board();
    int i;
    for(i = 0; i < game_turns; ++i) {
        if(i%garbage_interval == 0) { load_garbage(); }
        move_robot(fdm(get_board()));
        print_board();
        delay(500);
    }
}

void init_board() {
    board = (char**)malloc(board_height * sizeof(char*));
    int i, j;
    for(i = 0; i < board_height; ++i) {
        board[i] = (char*)malloc((board_width  + 1) * sizeof(char));
        for(j = 0; j < board_width; ++j) { board[i][j] = '.'; }
        board[i][board_width] = 0;
    }
    robotx = 0;
    roboty = 0;
    board[robotx][roboty] = 'X';
    score = 0;
}

void del_board() {
    int i;
    for(i = 0; i < board_height; ++i) { free(board[i]); }
    free(board);
}

char** get_board() { return board; }

void print_board() {
    lcd.clear();
    int i;
    for(i = 0; i < board_height; ++i) {
         lcd.setCursor(0, i);
         lcd.print(board[i]);
    }
    // printf("Score: %d\n", score);
}

void put_garbage(int x, int y) {
    if(board[x][y] == '.') { board[x][y] = 'o'; }
    else if(board[x][y] == 'X') { score += garbage_value; }
 }

void move_robot(int dir) {
    board[robotx][roboty] = '.';
    robotx += dirx[dir];
    roboty += diry[dir];
    if(robotx < 0) { robotx = 0; }
    else if(robotx >= board_height) { robotx = board_height - 1; }
    if(roboty < 0) { roboty = 0; }
    else if(roboty >= board_width) { roboty = board_width - 1; }
    if(board[robotx][roboty] == 'o') { score += garbage_value; }
    board[robotx][roboty] = 'X';
}

void load_garbage() {
    int j, x, y;
    for(j = 0; j < garbage_units; ++j) {
        int r1 = my_rand(board_height);
        int r2 = my_rand(board_width);  
        put_garbage((r1 + (1 - j%2)*3*j)%board_height, (r2 + (j%2)*3*j)%board_width);
    }
}

int fdm(char** board) { 
    int i, j, x, y;

    for(i = 0; i < board_height; ++i)
        for(j = 0; j < board_width; ++j)
            if(board[i][j] == 'X') { x = i; y = j; }

    int best = -1, mov = 0;

    for(i = 0; i < board_height; ++i)
        for(j = 0; j < board_width; ++j)
            if(board[i][j] == 'o') {
                int d = abs(i-x)+abs(j-y);
                if (best == -1 || d < best) {
                    best = d;
                    if (i == x)
                        mov = (y < j) ? 1 : 3;
                    else
                        mov = (x < i) ? 2 : 0;
                }
            }

    return mov;
}

int my_rand(int mod) {
    int d = rand();
    if(d < 0) { d *= -1; }
    return d % mod;
}
