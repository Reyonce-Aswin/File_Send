#include <stdlib.h>
#include "ui.h"
#include "transfer.h"
#include "discover.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

int main()
{
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    char choice[10];
    char file[100];
    char devices[10][20];

    init_ui();

    show_menu();
    prompt_input("Enter choice (1: Send, 2: Receive):", choice);

    if (choice[0] == '1')
    {
        int count = discover_devices(devices);

        show_devices(devices, count);

        if (count == 0)
        {
            prompt_input("No devices found", choice);
            close_ui();
            return 0;
        }

        prompt_input("Select device number (1 - <n>):", choice);
        int idx = atoi(choice) - 1;

        if (idx < 0 || idx >= count)
        {
            close_ui();
            return 0;
        }

        list_files();
        prompt_input("Enter file:", file);

        close_ui();
        run_sender_with_input(devices[idx], file);
    }
    else if (choice[0] == '2')
    {
        close_ui();
        run_receiver();
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}