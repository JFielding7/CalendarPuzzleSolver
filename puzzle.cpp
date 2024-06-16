//
// Created by joe on 6/15/24.
//
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_set>
#include "puzzle.h"

using namespace std;

void decode(board puzzle_board) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            printf("%c ", (char) ((puzzle_board >> (i * BOARD_WIDTH + j) & 1) + '0'));
        }
        puts("");
    }
}

void display_piece_key(puzzle_piece* pieces) {
    cout << "Piece Key: \n\n";
    for (int i = 0; i < PIECE_COUNT; i++) {
        for (int r = 0; r < pieces[i].height; r++) {
            for (int c = 0; c < pieces[i].width; c++) {
                if (pieces[i].piece_shape >> (r * BOARD_HEIGHT + c) & 1) {
                    printf("%c", i + 'A');
                }
                else cout << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
}

void decode_solution(const char* solution_board) {
    for (int r = 0; r < BOARD_HEIGHT; r++) {
        for (int c = 0; c < BOARD_WIDTH; c++) {
            cout << solution_board[r * BOARD_WIDTH + c] << " ";
        }
        cout << "\n";
    }
}

void reflect_piece(puzzle_piece* piece, puzzle_piece* reflected_piece) {
    int max = (piece->height - 1) * BOARD_WIDTH;
    for (int i = 0; i <= max; i+=BOARD_WIDTH) {
        reflected_piece->piece_shape |= (piece->piece_shape >> i & ROW_MASK) << (max - i);
    }
}

vector<puzzle_piece> get_rotations_and_reflections(puzzle_piece piece) {
    unordered_set<board> seen;
    vector<puzzle_piece> rotations;
    rotations.push_back(piece);
    for (int i = 0; i < ROTATIONS; i++) {
        puzzle_piece rotated_left = {.piece_shape = 0, .width = piece.height, .height = piece.width};
        for (int c0 = piece.width - 1, r1 = 0; r1 < piece.width; c0--, r1++) {
            for (int r0 = 0, c1 = 0; c1 < piece.height; r0++, c1++) {
                if((1lu << (r0 * BOARD_WIDTH + c0)) & piece.piece_shape) {
                    rotated_left.piece_shape |= (1lu << (r1 * BOARD_WIDTH + c1));
                }
            }
        }
        if (!seen.count(rotated_left.piece_shape)) {
            seen.insert(rotated_left.piece_shape);
            rotations.push_back(rotated_left);
        }
        puzzle_piece reflected = {.piece_shape = 0, .width = piece.width, .height = piece.height};
        reflect_piece(&piece, &reflected);
        if (!seen.count(reflected.piece_shape)) {
            seen.insert(reflected.piece_shape);
            rotations.push_back(reflected);
        }
        piece = rotated_left;
    }
    return rotations;
}

void place_piece(board location, int piece_idx, char* solution_board) {
    for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++) {
        if ((1lu << i) & location) solution_board[i] = (char) (piece_idx + 'A');
    }
}

static unsigned long x = 0b1111000000000000000000000011000011110001111000111;

board solve(board puzzle_board, char* solution_board, vector<vector<puzzle_piece>>& pieces, int piece_idx, int* pos) {
    (*pos)++;
    if (puzzle_board == FULL_BOARD) return true;
    if (piece_idx == PIECE_COUNT) return false;

    vector<puzzle_piece> rotations = pieces[piece_idx];
    int count = 0;
    for (puzzle_piece rot : rotations) {
        for (int row = 0; row < BOARD_HEIGHT - rot.height + 1; row++) {
            for (int col = 0; col < BOARD_WIDTH - rot.width + 1; col++) {
                cell curr_cell = row * BOARD_WIDTH + col;
                board location = rot.piece_shape << (curr_cell);
                count++;
//                if (piece_idx == 0) {
//                    cout << curr_cell << "\n";
//                    decode(location);
//                    puts("");
//                }
                if (!(puzzle_board & location)) {
                    if (solve(puzzle_board | location, solution_board, pieces, piece_idx + 1, pos)) {
                        place_piece(location, piece_idx, solution_board);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

board get_start_board(char* month, const char* date) {
    board start = EMPTY_BOARD;

    if (strlen(month) != MONTH_STR_LEN) return start;
    for (int i = 0; i < MONTH_STR_LEN; i++) month[i] = (char) tolower(month[i]);

    if (!strcmp(month, "jan")) start ^= JANUARY;
    if (!strcmp(month, "feb")) start ^= FEBRUARY;
    if (!strcmp(month, "mar")) start ^= MARCH;
    if (!strcmp(month, "apr")) start ^= APRIL;
    if (!strcmp(month, "may")) start ^= MAY;
    if (!strcmp(month, "jun")) start ^= JUNE;
    if (!strcmp(month, "jul")) start ^= JULY;
    if (!strcmp(month, "aug")) start ^= AUGUST;
    if (!strcmp(month, "sep")) start ^= SEPTEMBER;
    if (!strcmp(month, "oct")) start ^= OCTOBER;
    if (!strcmp(month, "nov")) start ^= NOVEMBER;
    if (!strcmp(month, "dec")) start ^= DECEMBER;
    int day = stoi(date);
    if (day > 0 && day < MONTH_DATE_BOUND) start ^= 1lu << (day + DATE_OFFSET);
    return start;
}

int main(int argc, char* argv[]) {
    puzzle_piece piece_arr[PIECE_COUNT] = PIECES;

    if (argc == 1 || argv[1][0] == '-') {
        display_piece_key(piece_arr);
        return 0;
    }

    if (argc != INPUT_ARG_COUNT) {
        cout << "Invalid. Follow this format:\n{first 3 letters of month} {day of month}\n";
        return 0;
    }
    board starting_board = get_start_board(argv[1], argv[2]);
    if (starting_board == FULL_BOARD || !((~starting_board) & (~starting_board - 1) & FULL_BOARD)) {
        cout << "Invalid date\n";
        return 0;
    }

    vector<vector<puzzle_piece>> pieces;
    pieces.reserve(PIECE_COUNT);
    for (int i = 0; i < PIECE_COUNT; i++) {
        pieces.push_back(get_rotations_and_reflections(piece_arr[i]));
    }

//    puzzle_piece** piece_types = (puzzle_piece**) malloc(PIECE_COUNT * sizeof (puzzle_piece*));
//    for (int i = 0; i < PIECE_COUNT; i++) {
//        piece_types[i] = (puzzle_piece*) malloc(pieces[i].size() * sizeof (puzzle_piece));
//        for (int j = 0; j < pieces[i].size(); j++) {
//            piece_types[i][j] = pieces[i][j];
//        }
//    }

    int pos = 0;
    int* pos_ptr = &pos;

    char* solution_board = (char*) malloc(BOARD_WIDTH * BOARD_HEIGHT);
    for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++) solution_board[i] = '-';

    clock_t start = clock();
    solve(starting_board, solution_board, pieces, 0, pos_ptr);
    clock_t end = clock();

    decode_solution(solution_board);
    cout << "\nTime: " << (double) (end - start) / CLOCKS_PER_SEC << "s";
    cout << "\nBoards Searched: " << *pos_ptr << "\n";
    return 0;
}

