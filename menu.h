#include <curses.h>
#include <string>

#ifndef MENU_H
#define MENU_H

#define ELEMENT_WIDTH 2
#define SPEED 150000
using namespace std;

class Menu {
public:
    Menu(string text, char trigger, string *items, int num_items) {
        this->text = text;
        this->trigger = trigger;
        this->items = items;
        this->num_items = num_items;
        this->selected_item = 0;
    }

    int start_x;
    string text;
    char trigger;

    string *items;
    int num_items;
    int selected_item;

    void selectNextItem() {
        selected_item++;
        if (selected_item >= num_items)
            selected_item = 0;
    }

    void selectPrevItem() {
        selected_item--;
        if (selected_item < 0)
            selected_item = num_items - 1;
    }
};

class MenuBar {
public:
    MenuBar(WINDOW* win, Menu *menus, int num_menus) {
        this->win = win;
        this->menus = menus;
        this->num_menus = num_menus;
        this->selected_menu = -1;

        int yMax, xMax, yBeg, xBeg;
        getmaxyx(win, yMax, xMax);
        getbegyx(win, yBeg, xBeg);

        menuwin = newwin(yMax - 2, xMax - 3, yBeg + 1, xBeg + 2);
        keypad(menuwin, true);
        wrefresh(menuwin);
        
        int current_pos = 2;

        for (int i = 0; i < num_menus; i++) {
            this->menus[i].start_x = current_pos;
            current_pos += this->menus[i].text.length() + 1;
        }
    }

    void reset() {
        for (int i = 0; i < num_menus; i++) {
            int start_x = menus[i].start_x;
            string text = menus[i].text;
            mvwprintw(win, 0, start_x, "%s", text.c_str());
        }
        wrefresh(win);
    }

    void draw() {
        for (int i = 0; i < num_menus; i++) {
            drawMenu(menus[i], selected_menu == i);
        }
        selected_menu = -1;
    }

    void drawMenu(Menu menu, bool is_selected) {
        int start_x = menu.start_x;
        string text = menu.text;
        if (is_selected) 
            wattron(win, A_STANDOUT);
        mvwprintw(win, 0, start_x, "%s", text.c_str());
        wattroff(win, A_STANDOUT);
        wrefresh(win);

        int ch;
        drawMenuItems(menu);
        wrefresh(menuwin);
        while (is_selected && (ch = wgetch(menuwin))) {
            switch (ch) {
                case 10:
                    executeFunction(menu);
                    is_selected = false;
                    break;
                case KEY_DOWN:
                    menu.selectNextItem();
                    break;
                case KEY_UP:
                    menu.selectPrevItem();
                    break;
                default:
                    is_selected = false;
                    break;
            }
            drawMenuItems(menu);
        }
        werase(menuwin);
        wrefresh(menuwin);
        reset();
    }

    void drawMenuItems(Menu menu) {
        int yMax, xMax;
        getmaxyx(menuwin, yMax, xMax);

        for (int i = 0; i < menu.num_items; i++) {
            mvwprintw(menuwin, i, 0, "%s", menu.items[i].c_str());
            if (menu.selected_item == i) {
                mvwchgat(menuwin, i, 0, xMax, A_NORMAL, 1, NULL);
            } else {
                mvwchgat(menuwin, i, 0, xMax, A_STANDOUT, 0, NULL);
            
            }
        }
    }

    void handleTrigger(char trigger) {
        for (int i = 0; i < num_menus; i++) {
            if (this->menus[i].trigger == trigger) {
                selected_menu = i;
            }
        }
    }

    void executeFunction(Menu menu) {
        vector<int> arr = {10, 2, 7, 1, 5, 5, 13, 3, 12, 11, 9, 8, 3, 4, 6, 11, 1, 3, 8,4};

        if (menu.text == "Algorithms") {
            if (menu.selected_item == 0) {
                bubbleSort(win, arr);
            } else if (menu.selected_item == 1) {
                selectionSort(win, arr);
            } else if (menu.selected_item == 2) {
                insertionSort(win, arr);
            }
        }
        if (menu.text == "Help") {
            help(win);
        }

        if (menu.text == "Info") {
            info(win);
        }
    }


    void info(WINDOW* win) {
        wclear(win);
        box(win, 0, 0);
        char ch;
        do {
            wattron(win, A_BOLD);
            mvwprintw(win, 2, 4, "Complexity");
            wattroff(win, A_BOLD);
            mvwprintw(win, 3, 4, "Bubble Sort: O(n^2)");
            mvwprintw(win, 4, 4, "Selection Sort: O(n^2)");
            mvwprintw(win, 5, 4, "Insertion Sort: O(n^2)");
            wattron(win, A_BOLD);
            mvwprintw(win, 7, 4, "Data Structure");
            wattroff(win, A_BOLD);
            mvwprintw(win, 8, 4, "Bubble Sort: Array");
            mvwprintw(win, 9, 4, "Selection Sort: Array");
            mvwprintw(win, 10, 4, "Insertion Sort: Array");
            wattron(win, A_BOLD);
            mvwprintw(win, 12, 4, "For exit press Enter");
            wattron(win, A_BOLD);
            wrefresh(win);
        } while (ch = wgetch(win) != 10);
    }

    void help(WINDOW* win) {
        wclear(win);
        box(win, 0, 0);
        char ch;
        do {
            wattron(win, A_BOLD);
            mvwprintw(win, 2, 12, "Algorithms Visualizer");
            wattroff(win, A_BOLD);
            mvwprintw(win, 3, 12, "Created by: Tsoi Vadim");
            mvwprintw(win, 4, 12, "  UID: 2022203502");
            mvwprintw(win, 7, 5, "This program is a visualizer of");
            mvwprintw(win, 8, 5, "algorithms Made as a project for the ");
            mvwprintw(win, 9, 5, "course of Data Structures.");

            mvwprintw(win, 11, 18, "               2023.12.19");
            wattron(win, A_BOLD);
            mvwprintw(win, 13, 11, "For exit press Enter");
            wattroff(win, A_BOLD);
            wrefresh(win);
        } while (ch = wgetch(win) != 10);
    }

    void bubbleSort(WINDOW* win, vector<int>& arr) {
        int n = arr.size();

        wrefresh(win);

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                
                // Clear the window
                wclear(win);
                box(win, 0, 0);

                // Draw the bars
                for (int k = 0; k < n; k++) {
                    wattron(win, COLOR_PAIR((k == j || k == j + 1) ? 4 : 3));

                    // Draw the bar
                    for (int l = 0; l < arr[k]; l++) {
                        mvwprintw(win, 13 - l, 3 + k * ELEMENT_WIDTH, "%c", ' ');
                    }

                    // Turn off highlighting
                    wattroff(win, COLOR_PAIR((k == j || k == j + 1) ? 4 : 3));
                }


                wrefresh(win);
                usleep(SPEED);

                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }

        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Sorted!");
        for (int i = 0; i < n; i++) {
            wattron(win, COLOR_PAIR(4));
            for (int l = 0; l < arr[i]; l++) {
                mvwprintw(win, 13 - l, 3 + i * ELEMENT_WIDTH, "%c", ' ');
            }
            wattroff(win, COLOR_PAIR(4));
        }
        wrefresh(win);

        usleep(2000000);
    }

    void selectionSort(WINDOW* win, vector<int>& arr) {
        int n = arr.size();
        
        wrefresh(win);
        usleep(1000000);

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;

            for (int j = i + 1; j < n; j++) {
                // Clear the window
                wclear(win);
                box(win, 0, 0);

                // Draw the bars
                for (int k = 0; k < n; k++) {
                    wattron(win, COLOR_PAIR((k == i || k == j) ? 4 : 3));

                    // Draw the bar
                    for (int l = 0; l < arr[k]; l++) {
                        mvwprintw(win, 13 - l, 3 + k * ELEMENT_WIDTH, "%c", ' ');
                    }

                    // Turn off highlighting
                    wattroff(win, COLOR_PAIR((k == i || k == j) ? 4 : 3));
                }

                wrefresh(win);
                usleep(SPEED);

                if (arr[j] < arr[minIndex]) {
                    minIndex = j;
                }
            }

            swap(arr[i], arr[minIndex]);
        }

        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Sorted!");
        for (int i = 0; i < n; i++) {
            wattron(win, COLOR_PAIR(4));
            for (int l = 0; l < arr[i]; l++) {
                mvwprintw(win, 13 - l, 3 + i * ELEMENT_WIDTH, "%c", ' ');
            }
            wattroff(win, COLOR_PAIR(4));
        }
        wrefresh(win);

        usleep(2000000);
    }

    void insertionSort(WINDOW* win, vector<int>& arr) {
        int n = arr.size();

        wrefresh(win);
        usleep(1000000);

        for (int i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;

            // Clear the window
            wclear(win);
            box(win, 0, 0);

            // Draw the bars
            for (int k = 0; k < n; k++) {
                wattron(win, COLOR_PAIR((k == i || k == j + 1) ? 4 : 3));

                // Draw the bar
                for (int l = 0; l < arr[k]; l++) {
                    mvwprintw(win, 13 - l, 3 + k * ELEMENT_WIDTH, "%c", ' ');
                }

                // Turn off highlighting
                wattroff(win, COLOR_PAIR((k == i || k == j + 1) ? 4 : 3));
            }

            wrefresh(win);
            usleep(SPEED);

            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;

                // Draw the bars during the shift
                wclear(win);
                box(win, 0, 0);

                // Draw the bars
                for (int k = 0; k < n; k++) {
                    wattron(win, COLOR_PAIR((k == i || k == j + 1) ? 4 : 3));

                    // Draw the bar
                    for (int l = 0; l < arr[k]; l++) {
                        mvwprintw(win, 13 - l, 3 + k * ELEMENT_WIDTH, "%c", ' ');
                    }

                    // Turn off highlighting
                    wattroff(win, COLOR_PAIR((k == i || k == j + 1) ? 4 : 3));
                }

                wrefresh(win);
                usleep(SPEED);
            }

            arr[j + 1] = key;
        }

        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "Sorted!");
        for (int i = 0; i < n; i++) {
            wattron(win, COLOR_PAIR(4));
            for (int l = 0; l < arr[i]; l++) {
                mvwprintw(win, 13 - l, 3 + i * ELEMENT_WIDTH, "%c", ' ');
            }
            wattroff(win, COLOR_PAIR(4));
        }
        wrefresh(win);

        usleep(2000000);
    }

    WINDOW* menuwin;
    WINDOW* win;
    Menu *menus;
    int num_menus;
    int selected_menu;
};

#endif