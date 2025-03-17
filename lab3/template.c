#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "pqueue.h"

char *filename = "queue.dat";

// Zmienna globalna do przechowywania identyfikatora przedmiotu
int itemId = 0;

typedef struct item {
    int id;
} item;

// Funkcja generująca nowy przedmiot (producent)
item *produce() {
    int time = rand() % 5 + 1;  // Generowanie losowego czasu (1-5 sekund)
    item *it = (item *)malloc(sizeof(item));  // Alokacja pamięci dla nowego przedmiotu
    sleep(time);  // Symulacja czasu produkcji
    it->id = itemId++;  // Ustawienie identyfikatora przedmiotu i inkrementacja globalnego itemId
    return it;  // Zwracamy wygenerowany przedmiot
}

// Funkcja symulująca konsumpcję przedmiotu
void consume(item *it) {
    int time = rand() % 5 + 1;  // Generowanie losowego czasu konsumpcji (1-5 sekund)
    sleep(time);  // Symulacja czasu konsumowania
    printf("Consumed item with ID: %d\n", it->id);  // Wypisanie komunikatu o skonsumowaniu przedmiotu
    free(it);  // Zwolnienie pamięci alokowanej dla przedmiotu
}

void producer(pid_t childPid) {
    pqueue *qu = NULL;  // Wskaźnik na kolejkę priorytetową, która będzie przechowywać przedmioty

    // Inicjalizacja kolejki przez deserializację z pliku
    qunserialize(&qu, sizeof(item), filename);

    while (1) {
        item *it = produce();  // Generowanie nowego przedmiotu
        qinsert(&qu, it, it->id);  // Dodanie przedmiotu do kolejki
        printf("Produced item with ID: %d\n", it->id);  // Wypisanie komunikatu o wyprodukowaniu przedmiotu
        qserialize(qu, sizeof(item), filename);  // Zapisanie aktualnego stanu kolejki do pliku
        sleep(1);  // Symulacja przerwy między produkcją przedmiotów
    }
}

void consumer() {
    pqueue *qu = NULL;  // Wskaźnik na kolejkę priorytetową

    // Inicjalizacja kolejki przez deserializację z pliku
    qunserialize(&qu, sizeof(item), filename);

    while (1) {
        // Sprawdzenie, czy kolejka zawiera elementy
        if (qu != NULL) {
            pqueue *pq_item = qpop(&qu);  // Pobranie pierwszego elementu z kolejki
            item *it = (item *)pq_item->data;  // Rzutowanie danych na odpowiedni typ przedmiotu
            consume(it);  // Konsumowanie przedmiotu
            free(pq_item);  // Zwolnienie pamięci alokowanej dla elementu kolejki
        }
        sleep(1);  // Czekaj na kolejne elementy do konsumpcji
    }
}

int main(int argc, char **argv) {
    pid_t pid;  // Zmienna do przechowywania identyfikatora procesu

    // Tworzenie procesu potomnego (producenta) za pomocą funkcji fork()
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");  // Obsługa błędu, gdy fork() się nie uda
        exit(1);
    }

    if (pid == 0) {
        producer(getppid());  // Wywołanie funkcji producenta
    } else {
        consumer();  // Wywołanie funkcji konsumenta
    }

    return 0;
}
