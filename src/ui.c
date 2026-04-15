#ifdef _WIN32
    #include <ncursesw/ncurses.h>
#else
    #include <ncurses.h>
#endif
#include <dirent.h>
#include <sys/stat.h>
#include "ui.h"
#include "discover.h"

WINDOW *left_win;
WINDOW *right_win;

void init_ui()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(1);

    int h, w;
    getmaxyx(stdscr, h, w);

    left_win = newwin(h, w / 2, 0, 0);
    right_win = newwin(h, w / 2, 0, w / 2);

    box(left_win, 0, 0);
    box(right_win, 0, 0);

    wrefresh(left_win);
    wrefresh(right_win);
}

void close_ui()
{
    endwin();
}

void show_menu()
{
    werase(left_win);
    box(left_win, 0, 0);

    mvwprintw(left_win, 2, 2, "1. Send");
    mvwprintw(left_win, 3, 2, "2. Receive");

    wrefresh(left_win);
}

void prompt_input(const char *msg, char *input)
{
    werase(right_win);
    box(right_win, 0, 0);

    mvwprintw(right_win, 2, 2, "%s", msg);
    mvwprintw(right_win, 3, 2, "> ");

    wrefresh(right_win);

    echo();
    wgetnstr(right_win, input, INPUT_MAX - 1);
    noecho();
}

void list_files()
{
    DIR *d = opendir(".");
    if (!d)
    {
        mvwprintw(left_win, 2, 2, "Error opening directory");
        wrefresh(left_win);
        return;
    }

    werase(left_win);
    box(left_win, 0, 0);

    mvwprintw(left_win, 1, 2, "Files:");

    int i = 3, max_y, max_x;
    getmaxyx(left_win, max_y, max_x);
    (void)max_x;
    
    struct dirent *dir;

    while ((dir = readdir(d)) != NULL)
    {
        struct stat st;

        if (stat(dir->d_name, &st) == 0 && S_ISREG(st.st_mode))
        {
            if (i >= max_y - 1) break;
            mvwprintw(left_win, i++, 2, "%s", dir->d_name);
        }
    }

    closedir(d);
    wrefresh(left_win);
}

void show_devices(Device devices[], int count)
{
    werase(left_win);
    box(left_win, 0, 0);

    mvwprintw(left_win, 1, 2, "Available Devices:");

    if (count == 0)
    {
        mvwprintw(left_win, 3, 2, "No devices found");
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            mvwprintw(left_win, i + 3, 2, "%d. %s", i + 1, devices[i].name);
        }
    }

    wrefresh(left_win);
}