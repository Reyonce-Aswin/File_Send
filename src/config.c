#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "config.h"

#define MAX_PATH 256

/* ---------------- NORMALIZE NAME ---------------- */
void normalize_device_name(char *name)
{
    int j = 0;

    for (int i = 0; name[i]; i++)
    {
        char c = name[i];

        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            (c == ' ') || (c == '-') || (c == '_'))
        {
            if (c == ' ')
                name[j++] = '-';   // replace space with '-'
            else
                name[j++] = c;
        }
    }

    name[j] = '\0';

    if (strlen(name) == 0)
        strcpy(name, "Device");
}

/* ---------------- BUILD CONFIG PATH ---------------- */
void get_config_path(char *path)
{
    const char *home = getenv("HOME");

    if (!home)
    {
        strcpy(path, ".config"); // fallback (rare case)
        return;
    }

    char dir[MAX_PATH];

    // ~/.config
    snprintf(dir, sizeof(dir), "%s/.config", home);
    mkdir(dir, 0755);

    // ~/.config/filesend
    snprintf(dir, sizeof(dir), "%s/.config/filesend", home);
    mkdir(dir, 0755);

    // final file path
    snprintf(path, MAX_PATH, "%s/.config/filesend/config", home);
}

/* ---------------- SAVE ---------------- */
void save_device_name(const char *name)
{
    char path[MAX_PATH];
    get_config_path(path);

    FILE *fp = fopen(path, "w");
    if (!fp) return;

    fprintf(fp, "initialized=1\n");
    fprintf(fp, "name=%s\n", name);

    fclose(fp);
}

/* ---------------- LOAD ---------------- */
int load_device_name(char *name)
{
    char path[MAX_PATH];
    get_config_path(path);

    FILE *fp = fopen(path, "r");
    if (!fp) return 0;

    char line[100];

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "name=", 5) == 0)
        {
            strcpy(name, line + 5);
            name[strcspn(name, "\n")] = 0;
        }
    }

    fclose(fp);
    return 1;
}