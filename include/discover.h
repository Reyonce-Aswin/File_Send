#ifndef DISCOVER_H
#define DISCOVER_H

typedef struct {
    char name[50];
    char ip[20];
} Device;

void set_device_name(const char *name);
void start_discovery_listener();
int discover_devices(Device devices[]);

#endif