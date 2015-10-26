#ifndef LIST_H
#define LIST_H

typedef struct listItem_t listItem_t;
typedef struct linkedList_t linkedList_t;

struct linkedList_t {
	void (*deleteCallback)(void*);
	listItem_t *head;
	listItem_t *tail;
	int count;
};

struct listItem_t {
	listItem_t *prev, *next;
	void *item;
};

#endif


void *createLinkedList(void (*)(void*));
void deleteLinkedList(linkedList_t*);
void *popListItem(linkedList_t*);
void *getListItem(linkedList_t*, int);
void clearList(linkedList_t*);
void addListItem(linkedList_t*, void*);
void deleteListItem(linkedList_t*, void*);