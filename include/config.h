#ifndef CONFIG_H
#define CONFIG_H

void save_device_name(const char *name);
int load_device_name(char *name);
void normalize_device_name(char *name);

#endif