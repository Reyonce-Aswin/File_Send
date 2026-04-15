#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transfer.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
#define CLOSESOCK closesocket
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef int socket_t;
#define CLOSESOCK close
#endif

#define PORT 5000

static void die(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static int send_all(socket_t s, const void *buf, int len)
{
    int sent = 0;
    const char *p = buf;

    while (sent < len)
    {
        int n = send(s, p + sent, len - sent, 0);
        if (n <= 0) return -1;
        sent += n;
    }
    return sent;
}

void run_sender_with_input(const char *ip, const char *filename)
{
    socket_t sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid < 0) die("socket");

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

#ifdef _WIN32
    server.sin_addr.s_addr = inet_addr(ip);
    if (server.sin_addr.s_addr == INADDR_NONE)
        die("Invalid IP");
#else
    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0)
        die("inet_pton");
#endif

    if (connect(sid, (struct sockaddr *)&server, sizeof(server)) < 0)
        die("connect");

    FILE *fp = fopen(filename, "rb");
    if (!fp) die("fopen");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    int fname_len = strlen(filename);

    if (send_all(sid, &fname_len, sizeof(int)) < 0 ||
        send_all(sid, filename, fname_len) < 0 ||
        send_all(sid, &size, sizeof(long)) < 0)
        die("send header");

    char buf[1024];
    int n;

    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
    {
        if (send_all(sid, buf, n) < 0)
            die("send data");
    }

    fclose(fp);
    CLOSESOCK(sid);
}