//
// Created on 25.12.2019 by Сергей Капшученко
//

#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>
#include <string.h>

#define X 10                                             // Size of main_window
#define Y 22                                             // Size of main_window

struct block current_block;                              // current block
struct block next_block;                                 // next block

int level, score, delay;                                 // init lvl, score, delay
bool hide = false;

char main_window[Y][X];                                  // main screen ( game, score, title windows)

WINDOW *game_window, *score_window, *title_window;       // separated windows for game, score and title

const char blocks[7][4][4][4] = {

        /*
            All block with theirs rotations
            each array for single block includes 4 rotations for that block
         */

        {                   // ----
                {
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                },
                {
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                },
                {
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
        },
        {               // Square []
                {
                        {1, 1, 0, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 1, 0, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 1, 0, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 1, 0, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
        },
        {                   // L
                {
                        {1, 1, 0, 0},
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 1, 1, 0},
                        {0, 0, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 0, 0, 0},
                        {1, 1, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
        },
        {                   // _|
                {
                        {1, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {0, 0, 1, 0},
                        {1, 1, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}
                },
                {
                        {1, 0, 0, 0},
                        {1, 0, 0, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 1, 1, 0},
                        {1, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
        },
        {                   // .:.
                {
                        {0, 1, 0, 0},
                        {1, 1, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 0, 0, 0},
                        {1, 1, 0, 0},
                        {1, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {       {1, 1, 1, 0},
                        {0, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {       {0, 1, 0, 0},
                        {1, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 0, 0, 0},
                },
        },
        {                   // S
                {
                        {0, 1, 0, 0},
                        {1, 1, 0, 0},
                        {1, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {       {1, 1, 0, 0},
                        {0, 1, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {0, 1, 0, 0},
                        {1, 1, 0, 0},
                        {1, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 1, 0, 0},
                        {0, 1, 1, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
        },
        {                   // Z
                {
                        {1, 0, 0, 0},
                        {1, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {0, 1, 1, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {1, 0, 0, 0},
                        {1, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 0, 0, 0},
                },
                {
                        {0, 1, 1, 0},
                        {1, 1, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                },
        },
};

struct position {                                       // struct to represent position of block
    short x, y;
};

struct block {                                          // struct to represent block
    int block;
    int rotation;
    struct position position;
};

struct block get_random_block() {

    /*
        Function that returns random block
     */

    struct block b = {
            .block = rand() % 7,
            .rotation = 0,
            .position = {
                    .x = 4,
                    .y = 3,
            },
    };
    return b;
}


void add_current_block() {

    /*
        Function that adds current block to the main_window
     */

    int y, x;

    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            if (blocks[current_block.block][current_block.rotation][y][x])
                main_window[current_block.position.y - y][current_block.position.x + x] = current_block.block + 1;
            // + 1 because colors starts from 0
            // colors are not random
        }
    }
}

void remove_current_block() {

    /*
        Function removes current block from main_window
    */

    int y, x;

    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            if (blocks[current_block.block][current_block.rotation][y][x])
                main_window[current_block.position.y - y][current_block.position.x + x] = 0;
        }
    }
}

chtype SPACE = ' ' | A_REVERSE | A_STANDOUT;
// constant colorful space for blocks
// we dont print letters, we print colorful SPACE

static const char TITLE[] =

        /*
          GAME TITLE - " TETRIS "
         */

        "111111  666666  555555  4444444  33  222222\n"
        "  22    11        66    55   55  44  33    \n"
        "  33    22222     11    666666   55  444444\n"
        "  44    33        22    11   11  66      55\n"
        "  55    444444    33    22    22 11  666666\n";

static const char INSTRUCTIONS[] =

        /*
            instruction
            controll buttons
         */
        " CONTROLL BUTTONS:  press ( h ) to hide  \n\n"
        " LEFT  : move left     (<--)               \n"
        " RIGHT : move right    (-->)               \n"
        " UP    : rotate block  (   )               \n"
        " DOWN  : move down     (   )               \n"
        " SPACE : fast down     ( _ )               \n"
        " P     : pause game    ( P )               \n"
        " Q     : quit          ( Q )               \n";


void print_title() {

    /*
     Function to print game title
     */

    const char *c;

    for (c = TITLE; *c != '\0'; c++) {
        if (*c == '\n' || *c == ' ') {
            waddch(title_window, *c);                              // if space or new line - just add char without color
        } else {

            /*
                we use - 48, because chars "123...0" starts from 49
                so color for char "1" will be 49 - 48 = 1
             */

            wattron(title_window, COLOR_PAIR(*c - 48));     // turn on color for window
            waddch(title_window, SPACE);                           // add char to the window ( colorful pixel)
            wattroff(title_window, COLOR_PAIR(*c - 48));    // turn off color for window
        }
    }
    wrefresh(title_window);                                        // refresh title window
}

void get_next_block(char next[4][4]) {

    int y, x;

    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            next[y][x] = blocks[next_block.block][next_block.rotation][y][x] ? next_block.block + 1 : 0;
            // if pos is true, then x + 1 else 0
            // + 1 because colors start from 1
        }
    }
}

void print_score() {

    /*
        Function to print score (border, score, ... )
     */

    int y, x;
    char next[4][4];

    werase(score_window);                                                         // clean score window ( "You lost...")

    mvwprintw(score_window, 1, 2, "NEXT BLOCK");

    mvwaddch(score_window, 2, 0, ACS_ULCORNER);                                 // print upper left corner
    mvwaddch(score_window, 2, 13, ACS_URCORNER);                                // print upper right corner
    mvwaddch(score_window, 7, 0, ACS_LLCORNER);                                // print lower left corner
    mvwaddch(score_window, 7, 13, ACS_LRCORNER);                               // print lower right corner

    for (x = 1; x < 13; x++) {
        mvwaddch(score_window, 2, x, ACS_HLINE);                                   // print horizontal line
        mvwaddch(score_window, 7, x, ACS_HLINE);                                   // print horizontal line
    }

    for (y = 3; y < 7; y++) {
        mvwaddch(score_window, y, 0, ACS_VLINE);                                    // print vertical line
        mvwaddch(score_window, y, 13, ACS_VLINE);                                   // print vertical line
    }


    get_next_block(next);

    /*
        print next block
     */

    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            chtype c = next[y][x] ? SPACE : ' ';                                // if x = 1 print color, else space
            wattron(score_window, COLOR_PAIR(next[y][x]));               // turn on color for window
            mvwaddch(score_window, 6 - y, 5 + x * 2, c);                 // add char
            mvwaddch(score_window, 6 - y, 6 + x * 2, c);                 // add char
            wattroff(score_window, COLOR_PAIR(next[y][x]));              // turn of color for window
        }
    }

    mvwprintw(score_window, 9, 1, "Level: %d                      ", level);            // print level
    mvwprintw(score_window, 10, 1, "Score: %d                      ", score);            // print score

    if (!hide) {
        /*
            Print controll buttons (instruction) if user wants to
         */
        mvwprintw(score_window, 12, 0, INSTRUCTIONS);                                       // print instruction
        mvwaddch(score_window, 16, 25, ACS_UARROW);                                  // add upper arrow
        mvwaddch(score_window, 17, 25, ACS_DARROW);                                  // add down arrow
    }
    wrefresh(score_window);                                                             // refresh window
}

void print_block() {

    /*
        Function prints current block
     */

    int y, x;

    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            if (main_window[y][x]) {
                wattron(game_window, COLOR_PAIR(main_window[y][x]));           // turn on color for game window
                mvwaddch(game_window, y + 1, x * 2 + 1, SPACE);                 // add colorful char
                mvwaddch(game_window, y + 1, x * 2 + 2, SPACE);                 // add colorful char
                wattroff(game_window, COLOR_PAIR(main_window[y][x]));          // turn off color for game window
            } else {
                mvwaddch(game_window, y + 1, x * 2 + 1, ' ');               // else add space
                mvwaddch(game_window, y + 1, x * 2 + 2, ' ');               // else add space
            }
        }
    }

    wrefresh(game_window);                                                             // refresh game window
}

void start_game() {
    memset(main_window, 0, sizeof(main_window));                                // put 0
    current_block = get_random_block();                                         // get current block
    next_block = get_random_block();                                            // get next block
    score = 0;                                                                  // init score and level
    level = 1;
    add_current_block();                                                        // add current block to the main_window
}

void draw_windows() {

    /*
        COLS it is an curses.h library variable
        shows amount of columns in terminal
     */

    int center = COLS / 2;

    int title_window_width = 44;                                                // title window width
    int title_window_height = 5;                                                // title window height
    int title_window_start_x = center - title_window_width / 2 - 5;             // title start X position
    int title_window_start_y = 1;                                               // title start Y position

    int game_window_start_x = center - X * 2 - 1;                               // game start X position
    int game_window_start_y = title_window_height + 2;                          // game start Y position
    int game_window_width = X * 2 + 2;                                          // game window width
    int game_window_height = Y + 2;                                             // game window height

    int score_window_start_x = game_window_start_x + game_window_width + 4;     // score start X position
    int score_window_start_y = game_window_start_y;                             // score start Y position
    int score_window_width = COLS - score_window_start_x;                       // score window width
    int score_window_height = game_window_height - 1;                           // score window height

    // create and return a pointers to the new windows 
    score_window = newwin(score_window_height, score_window_width, score_window_start_y, score_window_start_x);
    title_window = newwin(title_window_height, title_window_width, title_window_start_y, title_window_start_x);

    print_title();                                                           // draw main game title - "TETRIS"

    game_window = newwin(game_window_height, game_window_width, game_window_start_y, game_window_start_x);

//    wattron(game_window,    COLOR_PAIR(8));
    box(game_window, 0, 0);                                            // draw a box around the edges of a window
//    wattroff(game_window,   COLOR_PAIR(8));
}

void close_windows() {
    endwin();
}

void init_main_window(void) {

    initscr();                                                // init screen
    cbreak();                                                 // unbuffered input
    keypad(stdscr, TRUE);                                     // for special keys
    noecho();                                                 // do not echo character on screen
    curs_set(0);                                              // do not show cursor
    timeout(100);                                       // small delay

    atexit(close_windows);                                    // close curse windows on exit of program

    // color initialization
    // init_pair(index, foreground, background);

    use_default_colors();
    start_color();
    init_pair(1, 56, 0);
    init_pair(2, 160, 0);
    init_pair(3, 76, 0);
    init_pair(4, 135, 0);
    init_pair(5, 205, 0);
    init_pair(6, 220, 0);
    init_pair(7, 162, 0);
    init_pair(8, COLOR_BLACK, 0);

    refresh();                              // get actual output to the terminal

    draw_windows();                         // draw (init) game_window, title_window, score_window

    print_score();                          // print score
    print_block();                          // print block
}

int check_block_overlap() {

    /*
        Function that checks if block overlaps or not
        If overlaps - return 1
        else - return 0
    */

    int y, x;

    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            if (blocks[current_block.block][current_block.rotation][y][x]           // if x is 0, don't check
                && (current_block.position.y - y >= Y
                    || current_block.position.y - y < 0
                    || current_block.position.x + x >= X
                    || current_block.position.x + x < 0
                    || main_window[current_block.position.y - y][current_block.position.x + x]))
                // check if main_window already contains block
                return 1;
        }
    }
    return 0;
}

void offer_new_game() {
    chtype c;

    print_score();                      // draw score
    print_block();                      // draw block

    wclear(score_window);               // clear score window to print new 

    if (score < 500) {
        wprintw(score_window, "\nDude, you lost. Try to pass 1 level.\nYour score: %d\nYour level: %d\n", score, level);
    } else {
        wprintw(score_window, "You lost.\nYour score: %d\nYour level: %d\n\n", score, level);
    }

    wprintw(score_window, "Start a new game ? (y/n)");
    wrefresh(score_window);                                         // refresh score window

    while ((c = getch()) != 'y') {                                 // wait for user input
        if (c == 'n')
            exit(0);                                                // exit with success
    }

    start_game();                                                   // if 'y' - yes, then start a new game
}

int eliminate_lines() {

    /*
        Function to eliminate lines
    */

    int amount_of_eliminated_lines = 0;                                           // amount of eliminated lines
    int y, x, _y, _x;

    int score_for_line[] = {5, 50, 200, 500, 1000};                               // score for eliminated lines


    for (y = 0; y < Y; y++) {
        /*
            Looking for a line to eliminate and check if it is possible
         */
        for (x = 0; x < X; x++) {
            if (!main_window[y][x]) {                              // if main window doesnt contain point, then break
                break;
            }
        }

        if (x != X) {
            /*
                If x != X, then go check next line
             */
            continue;
        }


        for (_y = y; _y >= 3; _y--) {
            /*
                move all upper lines down
             */
            for (_x = 0; _x < X; _x++) {
                main_window[_y][_x] = main_window[_y - 1][_x];
                // h - 1, because Y coordinate increases, the point moves further down
            }
        }
        amount_of_eliminated_lines++;                                   // + 1 to eliminated lines
    }
    return score_for_line[amount_of_eliminated_lines];                  // return score
}

void check_bottom() {
    score += eliminate_lines();                         // eliminate lines and count score

    level = 1 + score / 500;                            // change level if it is possible

    current_block = next_block;                         // change current block for the next block
    next_block = get_random_block();

    if (check_block_overlap()) {                        // if new block overlaps in start pos (3,4) so you lost the game
        offer_new_game();                               // offer a new game 
        return;
    }

    add_current_block();
}

int _move_down() {

    /*
        Additional function for move_down()
     */

    int bottom = 0;

    remove_current_block();                 // remove current block from main_window
    current_block.position.y++;             // move our current block down

    if (check_block_overlap()) {            // if overlaps - bottom = 1, else 0
        bottom = 1;
        current_block.position.y--;         // if true, return previous position of block
    }
    add_current_block();                    // add current block to the main_window

    delay = 1000 - (level * 50);          // delay ( each level is harder than previous one

    return bottom;
}

void move_down() {

    /*
        Function that moves down our current block
     */

    if (_move_down()) {                                                // if True, check bottom blocks
        check_bottom();
    }
}

void move_left() {
    remove_current_block();                                             // remove current block from main window
    current_block.position.x--;
    if (check_block_overlap())                                          // if overlaps, return previous one
        current_block.position.x++;
    add_current_block();                                                // add current block to the main window
}

void move_right() {
    remove_current_block();                                             // remove current block from main window
    current_block.position.x++;
    if (check_block_overlap())                                          // if overlaps, return previous one
        current_block.position.x--;
    add_current_block();                                                // add current block to the main window
}

void rotate() {
    remove_current_block();                                             // remove current block from main window
    current_block.rotation = (current_block.rotation + 1) % 4;
    if (check_block_overlap()) {                                        // if overlaps, return previous rotation
        //printf("Current rotation before: %d\n", current_block.rotation);
        current_block.rotation = (current_block.rotation - 1) % 4;
        if (current_block.rotation < 0) current_block.rotation += 4;
        //printf("Current rotation after: %d\n", current_block.rotation);
    }
    add_current_block();                                                // add current block to the main window
}

void drop_block() {
    while (!_move_down())
        continue;
    check_bottom();
}

void loop() {

    init_main_window();

    while (true) {

        delay -= 100;                       // implementation of delay

        if (delay <= 0)

            move_down();                     // each delay move down our current block

        switch (getch()) {                   // check cases for clicked button
            case 'h':
                hide = !hide;
                break;
            case KEY_LEFT:                   // if left button
                move_left();                 // move left our current block
                break;
            case KEY_RIGHT:                  // if right button
                move_right();                // move right our current block
                break;
            case KEY_UP:                     // if up button
                rotate();                    // rotate our current block
                break;
            case KEY_DOWN:                   // if down button
                move_down();                 // move down our current block
                break;
            case ' ':                        // if space button
                drop_block();                // move down our current block till it doesnt overlap with ground/block
                break;
            case 'p':                        // if p button
                while (getch() != 'p');      // pausing the game while p is not clicked again
                break;
            case 'q':                        // if q button
                return;                      // return - end our program
        }

        print_score();                       // draw current score on score_window
        print_block();                       // draw current block on game_window
    }
}

int main() {
    srand((unsigned) time(NULL));
    start_game();
    loop();
}
