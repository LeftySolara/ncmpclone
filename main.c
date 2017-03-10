#include <ncurses.h>

int main() {

    /* Initialize ncurses */
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    keypad(stdscr, TRUE);

    int x = getmaxx(stdscr) / 2;
    int y = getmaxy(stdscr) / 2;
    mvprintw(y, x, "Hello world!");
    refresh();

    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        ;
    }

    endwin();

    return 0;
}
