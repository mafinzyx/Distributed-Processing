#include <stdio.h>
#include <stdlib.h>
#include "priority_queue_list.h"

void
qlist(pqueue *head, void (*print_data)(void *)) {
	pqueue *p;
	
	for (p = head; p != NULL; p = p->next) {
		printf("%d: ", p->k);
		print_data(p->data);
		printf("\n");
	}
	
}

void qinsert(pqueue **phead, void *data, int k) {
    pqueue *new_node = (pqueue *)malloc(sizeof(pqueue));
    new_node->data = data;
    new_node->k = k;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (*phead == NULL) {
        *phead = new_node;
        return;
    }

    pqueue *curr = *phead, *prev = NULL;
    while (curr != NULL && curr->k < k) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        new_node->next = *phead;
        (*phead)->prev = new_node;
        *phead = new_node;
    } else { 
        new_node->next = curr;
        new_node->prev = prev;
        prev->next = new_node;
        if (curr) curr->prev = new_node;
    }
}



void qremove(pqueue **phead, int k) {
    if (*phead == NULL) return;

    pqueue *curr = *phead;

    while (curr != NULL && curr->k != k) {
        curr = curr->next;
    }

    if (curr == NULL) return; 

    if (curr->prev) {
        curr->prev->next = curr->next;
    } else {
        *phead = curr->next; 
    }

    if (curr->next) {
        curr->next->prev = curr->prev;
    }

    free(curr->data);
    free(curr);
}


