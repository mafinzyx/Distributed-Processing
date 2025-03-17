#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    MessageBox(NULL, "To jest podproces.", "Info", MB_OK);
    return 0;
}

int main() {
    // Tworzenie procesu
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcess(NULL, "cmd.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        MessageBox(NULL, "Blad przy tworzeniu procesu.", "Blad", MB_OK);
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    MessageBox(NULL, "Podproces zakonczyl dzialanie.", "Info", MB_OK);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    HANDLE hThread = CreateThread(NULL, 0, MyThreadFunction, NULL, 0, NULL);
    if (hThread == NULL) {
        MessageBox(NULL, "Blad przy tworzeniu watku.", "Blad", MB_OK);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    MessageBox(NULL, "Zakonczono wszystkie procesy i watki.", "Info", MB_OK);

    return 0;
}
