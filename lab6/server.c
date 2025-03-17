#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") // Linkowanie z Winsock

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");

    // Tworzenie gniazda
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    // Konfiguracja struktury sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bindowanie
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Bind done.\n");

    // Nasłuchiwanie
    if (listen(server_socket, 3) < 0) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Waiting for incoming connections...\n");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("accept() failed with error code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Wysyłanie wiadomości do klienta
    char *message = "Hello Client, connection accepted!\n";
    send(client_socket, message, strlen(message), 0);

    // Zamknięcie połączeń
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
    