#include "server_utils.h"


void startSocket() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Impossible de lancer le socket sous Windows\n");
        exit(EXIT_FAILURE);
    }
#endif
}

void stopSocket() {
#ifdef _WIN32
    WSACleanup();
#endif
}


void ErrorMess(const char *error_str) {
    fprintf(stderr, "%s\n", error_str);
    exit(EXIT_FAILURE);
}

void parse_request(const char *buffer, REQUEST *request) {
    sscanf(buffer, "%s %s", request->method, request->url);
    if(strcmp(request->method, "POST") == 0){
        int json_start = -1;
        int json_end = -1;
        int len = strlen(buffer);
        for(int i=0;i<len;i++){
            if (buffer[i]=='{'){
                json_start = i;
                
            }
            

        }
        for(int i=json_start;i<len;i++){
            if (buffer[i]=='}' ){
                json_end = i;            
            }
            
        }
        // printf("%d %d\n",json_start, json_end);
        // if(json_start>json_end) return;
        for (int j=json_start;j<json_end+1; j++){
            // printf("%c", buffer[j]);
            request->json[j-json_start] = buffer[j];
        }
        // printf("\n%s\n",request->json);
        
    }

}

const char* get_mime_type(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return "application/octet-stream";

    if (strcmp(dot, ".html") == 0) return "text/html;";
    if (strcmp(dot, ".css") == 0) return "text/css";
    if (strcmp(dot, ".js") == 0) return "application/javascript";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(dot, ".png") == 0) return "image/png";
    if (strcmp(dot, ".gif") == 0) return "image/gif";
    if (strcmp(dot, ".json") == 0) return "application/json";
    if (strcmp(dot, ".svg") == 0) return "image/svg+xml";

    return "application/octet-stream";
}

RESPONSE manage_request(REQUEST request, ROUTE routes[], int num_routes, socket_t client_sock) {
    RESPONSE response;
    response.Client_sock = client_sock;
    response.status = 200;
    response.body= NULL;
    strcpy(response.message, "Not Found");
    response.file = NULL;
    response.file_size = 0;
    response.content = request.json; 

    strcpy(response.content_type, "text/html; charset=utf-8");
    if (strncmp(request.url, "/static/", 8) == 0) {
        char filepath[256] = "../static";
        strncat(filepath, request.url + 7, sizeof(filepath) - strlen(filepath) - 1);

        //validate_static_path(filepath);

        int bytes_read = read_file(filepath, &response.file, &response.file_size);
        if (bytes_read >= 0) {
            response.status = 200;
            strcpy(response.message, "OK");
            strcpy(response.content_type, get_mime_type(filepath));
        } else {
            response.status = 404;
            strcpy(response.message, "Not Found");
            response.file = render("404.html");
            response.file_size = strlen(response.file);
        }
        return response;
    }
    for (int i = 0; i < num_routes; i++) {
        if (strcmp(routes[i].url, request.url) == 0) {
            if (strcmp(routes[i].method, request.method) != 0) {
                response.status = 405;
                strcpy(response.message, "Method Not Allowed");
                response.file = render("405.html");
                response.file_size = strlen(response.file);
                strcpy(response.content_type, "text/html; charset=utf-8");
                return response;
            }

            routes[i].view_func(&response);
            return response;
        }
    }

    
    response.status = 404;
    strcpy(response.message, "Not Found");
    response.file = render("404.html");
    response.file_size = strlen(response.file);

    return response;
}

void send_response(socket_t client_sock, RESPONSE response) {
    size_t content_length = response.file_size > 0 ? response.file_size : response.body_size;

    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 %d %s\r\n"
             "Content-Length: %zu\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             response.status, response.message, content_length, response.content_type);

    send(client_sock, header, strlen(header), 0);

    
    if (response.file) {
        send(client_sock, response.file, response.file_size, 0);
    } else if (response.body) {
        send(client_sock, response.body, response.body_size, 0);
    }
}

void *handle_client(void *client_sock_ptr) {
    socket_t client_sock = *(socket_t *)client_sock_ptr;
    free(client_sock_ptr);

    char buffer[MAX_PER_REQUEST] = {0};
    int recv_out = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (recv_out > 0) {
        buffer[recv_out] = '\0';

        REQUEST request;
        parse_request(buffer, &request);

        // printf("--> : %s %s\n", request.method, request.url);

        RESPONSE response = manage_request(request, routes, num_routes, client_sock);
        send_response(client_sock, response);
    // Sleep(10000);

    }

    closesocket(client_sock);
    return NULL;
}

int read_file(const char *filepath, char **buffer, size_t *buffer_size) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    *buffer_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = malloc(*buffer_size);
    if (!*buffer) {
        fclose(file);
        return -1;
    }

    fread(*buffer, 1, *buffer_size, file);
    fclose(file);
    return 0;
}

char* render(const char *view_name) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s", VIEWS_DIR, view_name);

    char *content;
    size_t content_size;
    if (read_file(filepath, &content, &content_size) < 0) {
        return "<html><body><h1>404 - Page Not Found</h1></body></html>";
    }
    return content;
}

void validate_static_path(char *filepath) {
    
    if (strstr(filepath, "..")) {
        fprintf(stderr, "Tentative d'accès à un fichier en dehors du répertoire autorisé.\n");
        exit(EXIT_FAILURE);
    }
}
