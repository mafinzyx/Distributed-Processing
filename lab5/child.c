#include <windows.h>
#include <stdio.h>

#define PIPE_NAME "\\\\.\\pipe\\MyPipe"

void childProcess() {
    HANDLE hPipe;
    DWORD dwRead;
    char buffer[1024];

    // Otwieranie istniejącej rury
    hPipe =     (
        PIPE_NAME,            // nazwa rury
        GENERIC_READ,         // dostęp do odczytu
        0,                    // brak dziedziczenia
        NULL,                 // brak opcji zabezpieczeń
        OPEN_EXISTING,        // otwieranie istniejącej rury
        0,                    // brak dodatkowych opcji
        NULL                  // brak szablonu
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Blad przy otwieraniu rury. Kod bledu: %lu\n", GetLastError());
        return;
    }

    // Odbieranie danych z rury
    if (!ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL)) {
        printf("Blad przy odbieraniu danych. Kod bledu: %lu\n", GetLastError());
    } else {
        printf("Otrzymano wiadomosc: %s\n", buffer);
    }

    // Zamknięcie uchwytu rury
    CloseHandle(hPipe);
}

int main() {
    // Funkcja procesu dziecka
    childProcess();
    return 0;
}
