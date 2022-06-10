#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 3
#define ROWS SIZE
#define COLS SIZE

typedef enum {
    SHAPE_X = 0,
    SHAPE_O,
    COUNT_SHAPES,
} Shape;

typedef enum {
    CELL_EMPTY = 0,
    CELL_FULL,
} Cell;

char shape_chars[COUNT_SHAPES] = {
    [SHAPE_X] = 'x',
    [SHAPE_O] = 'o',
};

typedef struct {
    bool quit;
    Cell cells[ROWS][COLS];
    int filled_count;
    int cur_row;
    int cur_col;
    Shape shape;
} Game;

void game_render(const Game *game)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            bool selected = row == game.cur_row && col == game.cur_col;

            putc(selected ? '[' : ' ', stdout);
            Cell cell = game.cells[row][col];
            putc(cell == CELL_EMPTY ? '.' : shape_chars[cell - CELL_FULL], stdout);
            putc(selected ? ']' : ' ', stdout);
        }
        putc('\n', stdout);
    }
}

typedef int Outcome;

bool check_row(const Game *game, int row, Shape s)
{
    for (int col = 0; col < COLS; ++col) {
        if (game.cells[row][col] - CELL_FULL != s) {
            return false;
        }
    }
    return true;
}

bool check_col(const Game *game, int col, Shape s)
{
    for (int row = 0; row < ROWS; ++row) {
        if (game.cells[row][col] - CELL_FULL != s) {
            return false;
        }
    }
    return true;
}

bool check_main_diag(const Game *game, Shape s)
{
    for (int i = 0; i < SIZE; ++i) {
        if (game.cells[i][i] - CELL_FULL != s) {
            return false;
        }
    }
    return true;
}

bool check_sec_diag(const Game *game, Shape s)
{
    for (int i = 0; i < SIZE; ++i) {
        if (game.cells[i][SIZE - i - 1] - CELL_FULL != s) {
            return false;
        }
    }
    return true;
}

typedef int Outcome;

Outcome game_check_victory(Game *game)
{
    for (Shape s = 0; s < COUNT_SHAPES; ++s) {
        for (int i = 0; i < SIZE; ++i) {
            if (check_row(game, i, s)) return s + 1;
            if (check_col(game, i, s)) return s + 1;
        }
        if (check_main_diag(game, s)) return s + 1;
        if (check_sec_diag(game, s)) return s + 1;
    }
    return 0;
}

void game_move(Game *game, char c)
{
    switch (c) {
    case 'a':
        game.cur_col -= 1;
        break;
    case 'd':
        game.cur_col += 1;
        break;
    case 'w':
        game.cur_row -= 1;
        break;
    case 's':
        game.cur_row += 1;
        break;
    case 'q':
        game.quit = true;
        break;
    case ' ': {
        Cell *cell = &game.cells[game.cur_row][game.cur_col];
        if (*cell == CELL_EMPTY) {
            *cell = CELL_FULL + game.shape;
            game.filled_count += 1;

            Outcome outcome = game_check_victory(game);
            if (outcome) {
                game_render(game);
                printf("%c won\n", shape_chars[outcome-1]);
                game.quit = true;
            } else if (game.filled_count == ROWS*COLS) {
                game_render(game);
                printf("tie\n");
                game.quit = true;
            }

            game.shape = 1 - game.shape;
        }
    } break;

    default:
    {}
    }
    if (game.cur_col < 0) game.cur_col = 0;
    if (game.cur_col >= COLS) game.cur_col = COLS-1;
    if (game.cur_row < 0) game.cur_row = 0;
    if (game.cur_row >= ROWS) game.cur_row = ROWS-1;

}

int main(void)
{
    static Game game = {0};
    static char cmd[256];

    while (!game.quit) {
        game_render(&game);
        printf("> ");
        fgets(cmd, sizeof(cmd), stdin);
        size_t n = strlen(cmd);
        for (size_t i = 0; i < n; ++i) {
            game_move(&game, cmd[i]);
        }
    }

    return 0;
}
