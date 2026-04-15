#ifndef UI_H
#define UI_H

#ifdef _WIN32
    #include <ncursesw/ncurses.h>
#else
    #include <ncurses.h>
#endif

#include "discover.h"

#define INPUT_MAX 100

extern WINDOW *left_win;
extern WINDOW *right_win;

void init_ui();
void close_ui();

void show_menu();
void prompt_input(const char *msg, char *input);
void list_files();
void show_devices(Device devices[], int count);

#endif