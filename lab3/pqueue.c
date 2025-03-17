#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pqueue.h"

// Dodaje element do kolejki
void qinsert(pqueue **phead, void *data, int k) {
    pqueue *new_node = (pqueue *)malloc(sizeof(pqueue));
    new_node->k = k;
    new_node->data = malloc(sizeof(data));
    memcpy(new_node->data, data, sizeof(data));

    if (*phead == NULL) {
        *phead = new_node;
        new_node->next = new_node->prev = NULL;
    } else {
        pqueue *temp = *phead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
        new_node->prev = temp;
        new_node->next = NULL;
    }
}

// Wypisuje wszystkie elementy w kolejce
void qlist(pqueue *head, void (*print_data)(void *)) {
    pqueue *current = head;
    while (current != NULL) {
        print_data(current->data);
        current = current->next;
    }
}

// Usuwa element z kolejki na podstawie klucza
void qremove(pqueue **phead, int k) {
    pqueue *current = *phead;
    while (current != NULL) {
        if (current->k == k) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                *phead = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            free(current->data);
            free(current);
            return;
        }
        current = current->next;
    }
}

// Usuwa i zwraca pierwszy element z kolejki
pqueue *qpop(pqueue **phead) {
    if (*phead == NULL) {
        return NULL;
    }
    pqueue *temp = *phead;
    *phead = temp->next;
    if (*phead) {
        (*phead)->prev = NULL;
    }
    temp->next = NULL;
    return temp;
}

// Serializuje kolejkę do pliku
void qserialize(pqueue *head, size_t ndata, char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Error opening file for serialization");
        return;
    }

    pqueue *current = head;
    while (current != NULL) {
        fwrite(&(current->k), sizeof(int), 1, f);
        fwrite(current->data, ndata, 1, f);
        current = current->next;
    }
    fclose(f);
}

// Deserializuje kolejkę z pliku
void qunserialize(pqueue **phead, size_t ndata, char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error opening file for deserialization");
        return;
    }

    pqueue *new_node;
    while (!feof(f)) {
        new_node = (pqueue *)malloc(sizeof(pqueue));
        if (fread(&(new_node->k), sizeof(int), 1, f) != 1) break;
        
        new_node->data = malloc(ndata);
        if (fread(new_node->data, ndata, 1, f) != 1) break;

        new_node->next = *phead;
        new_node->prev = NULL;
        if (*phead) {
            (*phead)->prev = new_node;
        }
        *phead = new_node;
    }
    fclose(f);
}
