#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "transfer.h"
#include "discover.h"

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
#define MAX_FILENAME 100

static void die(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static int recv_all(socket_t s, void *buf, int len)
{
    int recvd = 0;
    char *p = buf;
    while (recvd < len)
    {
        int n = recv(s, p + recvd, len - recvd, 0);
        if (n <= 0) return -1;
        recvd += n;
    }
    return recvd;
}

void *discovery_thread(void *arg)
{
    start_discovery_listener();
    return NULL;
}

void run_receiver()
{
    socket_t sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid < 0) die("socket");

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sid, (struct sockaddr *)&server, sizeof(server)) < 0)
        die("bind");

    pthread_t tid;
    if (pthread_create(&tid, NULL, discovery_thread, NULL) != 0)
        die("pthread_create");

    pthread_detach(tid);

    if (listen(sid, 5) < 0)
        die("listen");

    printf("Waiting for file...\n");

    socket_t nsid = accept(sid, NULL, NULL);
    if (nsid < 0) die("accept");

    int fname_len;
    if (recv_all(nsid, &fname_len, sizeof(int)) < 0)
        die("recv fname_len");

    if (fname_len <= 0 || fname_len >= MAX_FILENAME)
        die("bad fname_len");

    char filename[MAX_FILENAME];
    if (recv_all(nsid, filename, fname_len) < 0)
        die("recv filename");

    filename[fname_len] = '\0';

    long size;
    if (recv_all(nsid, &size, sizeof(long)) < 0)
        die("recv size");

    FILE *fp = fopen(filename, "wb");
    if (!fp) die("fopen");

    char buf[1024];
    long recvd = 0;

    while (recvd < size)
    {
        int n = recv(nsid, buf, sizeof(buf), 0);
        if (n <= 0) die("recv data");

        fwrite(buf, 1, n, fp);
        recvd += n;
    }

    fclose(fp);
    CLOSESOCK(nsid);
    CLOSESOCK(sid);
}