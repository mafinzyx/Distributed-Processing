#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define PIPE_NAME "\\\\.\\pipe\\MyPipe"

// Funkcja, która działa w procesie rodzica (tworzy rurę i wysyła dane)
void parentProcess() {
    HANDLE hPipe;
    DWORD dwWritten;
    const char *message = "Hello from parent process!";

    // Tworzenie pipe (rury)
    hPipe = CreateNamedPipe(
        PIPE_NAME,           // nazwa rury
        PIPE_ACCESS_OUTBOUND, // dostęp do zapisu
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, // tryb operacji
        1,                    // liczba instancji
        1024,                 // rozmiar bufora dla danych
        1024,                 // rozmiar bufora dla danych
        0,                    // czas oczekiwania
        NULL                  // brak dodatkowych opcji
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Blad przy tworzeniu rury. Kod bledu: %lu\n", GetLastError());
        return;
    }

    printf("Czekanie na polaczenie z klientem...\n");

    // Czekanie na połączenie z klientem (procesem dziecka)
    if (!ConnectNamedPipe(hPipe, NULL)) {
        printf("Blad przy laczeniu z klientem. Kod bledu: %lu\n", GetLastError());
        return;
    }

    // Wysyłanie danych do procesu dziecka
    if (!WriteFile(hPipe, message, strlen(message) + 1, &dwWritten, NULL)) {
        printf("Blad przy wysylaniu danych. Kod bledu: %lu\n", GetLastError());
    } else {
        printf("Wyslano wiadomosc: %s\n", message);
    }

    // Zamknięcie uchwytu rury
    CloseHandle(hPipe);
}

// Funkcja, która działa w procesie dziecka (odbiera dane z rury)
void childProcess() {
    HANDLE hPipe;
    DWORD dwRead;
    char buffer[1024];

    // Otwieranie istniejącej rury
    hPipe = CreateFile(
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
    // Utworzenie procesu dziecka
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));

    // Tworzenie procesu dziecka
    if (CreateProcess(
        NULL,                  // ścieżka do aplikacji
        "child.exe",            // argument - uruchomienie pliku dziecka
        NULL,                  // brak opcji zabezpieczeń
        NULL,                  // brak opcji zabezpieczeń
        FALSE,                 // brak dziedziczenia uchwytów
        0,                     // brak specjalnych flag
        NULL,                  // środowisko
        NULL,                  // katalog roboczy
        &si,                   // struktura STARTUPINFO
        &pi                    // struktura PROCESS_INFORMATION
    )) {
        printf("Stworzono proces dziecka.\n");
    } else {
        printf("Blad przy tworzeniu procesu dziecka. Kod bledu: %lu\n", GetLastError());
        return 1;
    }

    // Proces rodzica wysyła dane do dziecka
    parentProcess();

    // Czekamy na zakończenie procesu dziecka
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Zamknięcie uchwytów
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Zakonczono komunikacje miedzy procesami.\n");

    return 0;
}
