#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #include <windows.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/socket.h>
#endif

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1112
#define BUFFER_SIZE 1024

void startSockOnWindows(){
    #ifdef _WIN32
        WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa)!=0){
        exit(1);
    }
    #else
        printf("On linux");
        #define INVALID_SOCKET -1
    #endif
    
}

void freeResourcesC(SOCKET Sock){
    #ifdef _WIN32
        closesocket(Sock);
    #else
    {
        close(Sock);
    }
    #endif
}