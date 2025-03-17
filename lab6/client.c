#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") // Linkowanie z Winsock

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char buffer[1024] = {0};
    int recv_size;

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");

    // Tworzenie gniazda
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Połączenie lokalne
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    // Próba połączenia z serwerem
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Connected to server.\n");

    // Odbieranie wiadomości od serwera
    if ((recv_size = recv(client_socket, buffer, sizeof(buffer), 0)) == SOCKET_ERROR) {
        printf("recv failed. Error Code: %d\n", WSAGetLastError());
    } else {
        buffer[recv_size] = '\0';
        printf("Server reply: %s\n", buffer);
    }

    // Zamknięcie gniazda
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
