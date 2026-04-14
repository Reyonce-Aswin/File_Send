#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#ifdef _WIN32
#include <winsock2.h>
typedef SOCKET socket_t;
#else
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#define closesocket close
#endif

#define PORT 6000

void start_discovery_listener()
{
    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return;

    struct sockaddr_in server = {0}, client;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        return;

    char buffer[1024];
    int len = sizeof(client);

    while (1)
    {
        int n = recvfrom(sock, buffer, sizeof(buffer), 0,
                         (struct sockaddr *)&client, &len);

        if (n > 0 && strncmp(buffer, "DISCOVER", 8) == 0)
        {
            sendto(sock, "RECEIVER", 8, 0,
                   (struct sockaddr *)&client, len);
        }
    }
}

int discover_devices(char devices[][20])
{
    socket_t sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return 0;

    int opt = 1;

#ifdef _WIN32
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char *)&opt, sizeof(opt));
#else
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
#endif

    struct sockaddr_in addr = {0}, from;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    sendto(sock, "DISCOVER", 8, 0,
           (struct sockaddr *)&addr, sizeof(addr));

    struct timeval tv = {2, 0};

#ifdef _WIN32
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
#else
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif

    int count = 0, len = sizeof(from);
    char buf[1024];

    while (1)
    {
        int n = recvfrom(sock, buf, sizeof(buf), 0,
                         (struct sockaddr *)&from, &len);

        if (n <= 0) break;

        strncpy(devices[count], inet_ntoa(from.sin_addr), 20);
        devices[count][19] = '\0';

        count++;
        if (count >= 10) break;
    }

    closesocket(sock);
    return count;
}