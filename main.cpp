#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#endif

#define PORT 9090
#define BUFFER_SIZE 1024
#define LOG_FILE "server.log"

#ifdef _WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif



std::string get_content_type(const std::string &path) {
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".css") != std::string::npos) return "text/css";
    if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos) return "image/jpeg";
    if (path.find(".png") != std::string::npos) return "image/png";
    if (path.find(".gif") != std::string::npos) return "image/gif";
    if (path.find(".js") != std::string::npos) return "application/javascript";
    return "text/plain";
}


bool file_exists(const std::string &path) {
#ifdef _WIN32
    DWORD file_attr = GetFileAttributes(path.c_str());
    return (file_attr != INVALID_FILE_ATTRIBUTES && !(file_attr & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat file_stat;
    return (stat(path.c_str(), &file_stat) == 0 && !S_ISDIR(file_stat.st_mode));
#endif
}


void log_request(const std::string &method, const std::string &path, const std::string &status) {
    std::ofstream log_file(LOG_FILE, std::ios::app);
    if (log_file) {
        log_file << "Método: " << method << " | Caminho: " << path << " | Status: " << status << "\n";
        log_file.close();
    }
}


void send_response(socket_t client_socket, const std::string &status, const std::string &content_type, const std::string &body) {
    std::string header =
        "HTTP/1.1 " + status + "\r\n" +
        "Content-Type: " + content_type + "\r\n" +
        "Content-Length: " + std::to_string(body.size()) + "\r\n" +
        "Connection: close\r\n\r\n";
    send(client_socket, header.c_str(), header.size(), 0);
    send(client_socket, body.c_str(), body.size(), 0);
}


void handle_request(socket_t client_socket) {
    char buffer[BUFFER_SIZE];
    std::string method, path;


    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

    std::istringstream request(buffer);
    request >> method >> path;


    if (path.find("..") != std::string::npos) {
        send_response(client_socket, "400 Bad Request", "text/plain", "Caminho inválido.");
        log_request(method, path, "400 Bad Request");
#ifdef _WIN32
        closesocket(client_socket);
#else
        close(client_socket);
#endif
        return;
    }


    if (method == "GET") {
        if (path == "/") {
            path = "./index.html";
        } else {
            path = "." + path;
        }

        if (!file_exists(path)) {
            send_response(client_socket, "404 Not Found", "text/plain", "Arquivo não encontrado.");
            log_request(method, path, "404 Not Found");
#ifdef _WIN32
            closesocket(client_socket);
#else
            close(client_socket);
#endif
            return;
        }

        std::ifstream file(path, std::ios::binary);
        if (!file) {
            send_response(client_socket, "500 Internal Server Error", "text/plain", "Erro interno do servidor.");
            log_request(method, path, "500 Internal Server Error");
#ifdef _WIN32
            closesocket(client_socket);
#else
            close(client_socket);
#endif
            return;
        }

        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        send_response(client_socket, "200 OK", get_content_type(path), file_content);
        log_request(method, path, "200 OK");

    } else if (method == "POST") {
        std::string body = buffer + std::string(buffer).find("\r\n\r\n") + 4;
        std::cout << "Dados do POST: " << body << std::endl;

        send_response(client_socket, "200 OK", "text/plain", "POST recebido com sucesso.");
        log_request(method, path, "200 OK");

    } else {
        send_response(client_socket, "405 Method Not Allowed", "text/plain", "Método não permitido.");
        log_request(method, path, "405 Method Not Allowed");
    }

#ifdef _WIN32
    closesocket(client_socket);
#else
    close(client_socket);
#endif
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erro ao inicializar WinSock.\n";
        return 1;
    }
#endif

    socket_t server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Erro ao criar socket.\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Erro ao vincular socket.\n";
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Erro ao colocar socket em modo de escuta.\n";
        return 1;
    }

    std::cout << "Servidor rodando na porta " << PORT << "...\n";

    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);

    while (true) {
        socket_t client_socket = accept(server_socket, (sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            std::cerr << "Erro ao aceitar conexão.\n";
            continue;
        }

        handle_request(client_socket);
    }

#ifdef _WIN32
    closesocket(server_socket);
    WSACleanup();
#else
    close(server_socket);
#endif

    return 0;
}
