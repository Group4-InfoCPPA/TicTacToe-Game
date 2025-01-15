#ifndef SERVER_H
#define SERVER_H

    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>
    #include "dep_sock.h"
    #include "urls.h"
    

    
    

    

    


    void startSocket();
    void stopSocket();
    void ErrorMess(const char *error_str);

    RESPONSE manage_request(REQUEST request, ROUTE routes[], int num_routes, socket_t client_sock);
    void send_response(socket_t client_sock, RESPONSE response);
    void parse_request(const char *buffer, REQUEST *request);
    void *handle_client(void *client_sock_ptr);
    int read_file(const char *filepath, char **buffer, size_t *buffer_size);
    const char* get_mime_type(const char *filename);
    void format_str_res(char *dest, int status, const char *status_msg, size_t content_length, const char *content_type, const char *body);
    char* render(const char *view_name);
    void validate_static_path(char *filepath);
#endif // !SERVER_H
