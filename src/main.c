#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "transfer.h"
#include "discover.h"
#include "config.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

int main()
{
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    char device_name[50];

    if (!load_device_name(device_name))
    {
        init_ui();
        prompt_input("Enter device name:", device_name);
        normalize_device_name(device_name);
        save_device_name(device_name);
        close_ui();
    }

    set_device_name(device_name);

    char choice[10];
    char file[100];
    Device devices[10];

    int exit_code = 0;

    init_ui();

    show_menu();
    prompt_input("Enter choice (1: Send, 2: Receive, 3: Rename):", choice);

    if (choice[0] == '1')
    {
        int count = discover_devices(devices);

        show_devices(devices, count);

        if (count == 0)
        {
            prompt_input("No devices found", choice);
            goto cleanup;
        }

        prompt_input("Select device number:", choice);
        int idx = atoi(choice) - 1;

        if (idx < 0 || idx >= count)
        {
            goto cleanup;
        }

        list_files();
        prompt_input("Enter file:", file);

        close_ui();
        run_sender_with_input(devices[idx].ip, file);
    }
    else if (choice[0] == '2')
    {
        close_ui();
        run_receiver();
    }
    else if (choice[0] == '3')
    {
        char new_name[50];

        prompt_input("Enter new device name:", new_name);
        normalize_device_name(new_name);
        save_device_name(new_name);
    }

#ifdef _WIN32
    WSACleanup();
#endif
cleanup:
    close_ui();
    return exit_code;
}