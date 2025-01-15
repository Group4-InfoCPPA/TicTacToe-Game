#ifndef DEP_H
    #define DEP_H

    #ifdef _WIN32
        #include <WinSock2.h>
        #include <WS2tcpip.h>
        #define socket_t SOCKET
        #define inv_sock INVALID_SOCKET
    #else
        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <unistd.h>
        #define socket_t int
        #define closesocket close
        #define inv_sock -1
    #endif
#endif
