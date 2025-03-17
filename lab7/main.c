#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

// Struktura, która przekazuje zakres liczb do wątku
typedef struct {
    int start;
    int end;
} Range;

// Struktura do przechowywania wyników
typedef struct {
    int* primes;   // Wskaźnik na tablicę liczb pierwszych
    int size;      // Liczba znalezionych liczb pierwszych
} PrimeList;

// Funkcja sprawdzająca, czy liczba jest pierwsza
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}

// Funkcja wykonywana przez każdy wątek
void* find_primes(void* arg) {
    Range* range = (Range*)arg;
    PrimeList* result = (PrimeList*)malloc(sizeof(PrimeList));
    result->primes = (int*)malloc((range->end - range->start + 1) * sizeof(int));
    result->size = 0;

    for (int i = range->start; i <= range->end; i++) {
        if (is_prime(i)) {
            result->primes[result->size++] = i;
        }
    }

    pthread_exit(result);
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int start, end, num_threads;

    // Wczytanie zakresu liczb
    printf("Podaj poczatek zakresu: ");
    scanf("%d", &start);
    printf("Podaj koniec zakresu: ");
    scanf("%d", &end);

    // Liczba wątków
    printf("Podaj liczbe watkow: ");
    scanf("%d", &num_threads);

    // Obliczanie zakresu dla każdego wątku
    int range_size = (end - start + 1) / num_threads;
    pthread_t threads[num_threads];
    Range ranges[num_threads];
    PrimeList* results[num_threads];

    // Tworzenie wątków
    for (int i = 0; i < num_threads; i++) {
        ranges[i].start = start + i * range_size;
        ranges[i].end = (i == num_threads - 1) ? end : (start + (i + 1) * range_size - 1);
        
        if (pthread_create(&threads[i], NULL, find_primes, (void*)&ranges[i]) != 0) {
            perror("Blad tworzenia watku");
            return 1;
        }
    }

    // Czekanie na zakończenie wszystkich wątków i zbieranie wyników
    for (int i = 0; i < num_threads; i++) {
        void* thread_result;
        pthread_join(threads[i], &thread_result);
        results[i] = (PrimeList*)thread_result;
    }

    // Liczenie liczby wszystkich liczb pierwszych
    int total_size = 0;
    for (int i = 0; i < num_threads; i++) {
        total_size += results[i]->size;
    }

    // Tworzenie tablicy wyników i kopiowanie liczb pierwszych do niej
    int* primes = (int*)malloc(total_size * sizeof(int));
    int idx = 0;
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < results[i]->size; j++) {
            primes[idx++] = results[i]->primes[j];
        }
        free(results[i]->primes);
        free(results[i]);
    }

    // Sortowanie liczb pierwszych
    qsort(primes, total_size, sizeof(int), compare);

    // Wyświetlanie posortowanych liczb pierwszych
    printf("Liczby pierwsze w przedziale [%d, %d]:\n", start, end);
    for (int i = 0; i < total_size; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    free(primes);

    return 0;
}
