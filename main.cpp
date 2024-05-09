#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <algorithm>
#include "menu.h"


using namespace std;

int main(int argc, char **argv) {
    initscr();
    noecho();
    curs_set(0);

    if (!has_colors()) {
        printf("Your terminal does not support color\n");
        return 1;
    }

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *win = newwin(5+10, 5+39, 5, 10);
    box(win, 0, 0);

    string menu1[3] = { "Bubble Sort", "Selection Sort", "Insertion Sort" };
    string menu2[5] = { "Complexity", "Data Structure" };

    Menu menus[] = {
        Menu("Algorithms", 'a', menu1, 3),
        Menu("Info", 'i', NULL, 0),
        Menu("Help", 'h', NULL, 0),
    };

    MenuBar menubar = MenuBar(win, menus, 4);
    menubar.draw();

    char ch;
    while (ch = wgetch(win)) {
        menubar.handleTrigger(ch);
        menubar.draw();
        if (ch == 'q') {
            break;
        }

    }

    endwin();

    return 0;
}