#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 1024

// Funkcja, która zmienia wszystkie małe litery na duże
void to_uppercase(int read_fd, int write_fd) {
    char buffer[MAX_BUF];
    int bytes_read;

    // Odczytujemy dane z poprzedniego procesu przez potok
    while ((bytes_read = read(read_fd, buffer, sizeof(buffer))) > 0) {
        // Zamieniamy małe litery na duże
        for (int i = 0; i < bytes_read; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        // Przekazujemy wynik do następnego procesu
        write(write_fd, buffer, bytes_read);
    }
}

// Funkcja, która wyświetla otrzymany ciąg
void print_output(int read_fd) {
    char buffer[MAX_BUF];
    int bytes_read;

    // Odczytujemy dane z poprzedniego procesu
    while ((bytes_read = read(read_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
}

int main() {
    int pipe1[2], pipe2[2];  // Dwa potoki: pipe1 - proces 1 -> proces 2, pipe2 - proces 2 -> proces 3
    pid_t pid1, pid2;

    // Tworzymy pierwszy potok
    if (pipe(pipe1) == -1) {
        perror("Pipe1 creation failed");
        return 1;
    }

    // Tworzymy drugi potok
    if (pipe(pipe2) == -1) {
        perror("Pipe2 creation failed");
        return 1;
    }

    // Tworzymy pierwszy proces (proces 1)
    pid1 = fork();
    if (pid1 == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid1 == 0) {  // Proces 1: wczytuje dane i przekazuje do potoku pipe1
        close(pipe1[0]);  // Zamykamy niepotrzebny deskryptor do odczytu
        char input[MAX_BUF];

        printf("Podaj ciąg znaków: ");
        fgets(input, MAX_BUF, stdin);

        // Przekazujemy dane do potoku
        write(pipe1[1], input, strlen(input));
        close(pipe1[1]);  // Zamykamy deskryptor zapisu po wysłaniu danych

        exit(0);
    }

    // Tworzymy drugi proces (proces 2)
    pid2 = fork();
    if (pid2 == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid2 == 0) {  // Proces 2: zmienia małe litery na duże
        close(pipe1[1]);  // Zamykamy deskryptor zapisu potoku 1
        close(pipe2[0]);  // Zamykamy niepotrzebny deskryptor do odczytu potoku 2

        to_uppercase(pipe1[0], pipe2[1]);  // Przekazujemy dane do procesu 3

        close(pipe1[0]);  // Zamykamy deskryptor odczytu potoku 1
        close(pipe2[1]);  // Zamykamy deskryptor zapisu potoku 2

        exit(0);
    }

    // Proces 3: wyświetla otrzymany ciąg
    close(pipe1[0]);  // Zamykamy niepotrzebny deskryptor odczytu potoku 1
    close(pipe1[1]);  // Zamykamy niepotrzebny deskryptor zapisu potoku 1
    close(pipe2[1]);  // Zamykamy niepotrzebny deskryptor zapisu potoku 2

    print_output(pipe2[0]);  // Odczytujemy dane i wyświetlamy je na ekranie
    close(pipe2[0]);  // Zamykamy deskryptor odczytu potoku 2

    // Czekamy na zakończenie procesów potomnych
    wait(NULL);
    wait(NULL);

    return 0;
}
