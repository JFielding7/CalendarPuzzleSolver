//
// Created by joe on 6/15/24.
//

#ifndef CALENDARPUZZLESOLVER_PUZZLE_H
#define CALENDARPUZZLESOLVER_PUZZLE_H

#define cell unsigned long
#define shape unsigned long
#define ROTATIONS 4
#define board unsigned long
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 7
#define EMPTY_BOARD 0b1111000000000000000000000000000000010000001000000
//  0b1111000_0000000_0000000_0000000_0000000_1000000_1000000
#define FULL_BOARD ((1lu << 49) - 1)
#define ROW_MASK 0b1111111
#define PIECE_COUNT 8
#define PIECES {{.piece_shape = 0b1110000111, .width = 3, .height = 2},\
                {.piece_shape = 0b1110000011, .width = 3, .height = 2},\
                {.piece_shape = 0b1010000111, .width = 3, .height = 2},\
                {.piece_shape = 0b100000010000111, .width = 3, .height = 3},\
                {.piece_shape = 0b100001110000100, .width = 3, .height = 3},\
                {.piece_shape = 0b110001110, .width = 4, .height = 2},\
                {.piece_shape = 0b100001111, .width = 4, .height = 2},\
                {.piece_shape = 0b10001111, .width = 4, .height = 2}};

#define RESET    "\033[0m"
#define DARK_RED "\033[41m"
#define YELLOW   "\033[43m"
#define RED      "\033[101m"
#define GREEN    "\033[102m"
#define ORANGE   "\033[103m"
#define BLUE     "\033[104m"
#define MAGENTA  "\033[105m"
#define CYAN     "\033[106m"
#define WHITE    "\033[107m"

#define PIECE_COLORS {DARK_RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, ORANGE, RED, RESET, WHITE};

#define JANUARY 0b1
#define FEBRUARY 0b10;
#define MARCH 0b100;
#define APRIL 0b1000;
#define MAY 0b10000;
#define JUNE 0b100000;
#define JULY 0b10000000;
#define AUGUST 0b100000000;
#define SEPTEMBER 0b1000000000;
#define OCTOBER 0b10000000000;
#define NOVEMBER 0b100000000000;
#define DECEMBER 0b1000000000000;

#define MONTH_DATE_BOUND 32
#define DATE_OFFSET 13

#define INPUT_ARG_COUNT 3
#define MONTH_STR_LEN 3

typedef struct piece_s {
    shape piece_shape;
    int width;
    int height;
} puzzle_piece;



#endif //CALENDARPUZZLESOLVER_PUZZLE_H
