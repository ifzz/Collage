#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"


void *createLinkedList(void (*deleteCallback)(void*)) {
	linkedList_t *newListItem;

	newListItem = malloc(sizeof(linkedList_t));

	if (newListItem == NULL) {
		puts("**FATAL** Could not create linked list.");

		assert(1 == 2);
	}

	newListItem->count = 0;
	newListItem->deleteCallback = deleteCallback;
	newListItem->head = NULL;
	newListItem->tail = NULL;

	return newListItem;
}

void deleteLinkedList(linkedList_t *list) {
	clearList(list);
	
	free(list);
}

void _removeListItem(linkedList_t *list, listItem_t *listItem){
	if (list->head != listItem && list->tail != listItem) {
		//printf("Deleting middle! (C=%i)\n", list->count);
		
		//assert(listItem->next != NULL);
		
		//if (list->count > 1) {
			listItem->prev->next = listItem->next;
			listItem->next->prev = listItem->prev;
		//}
	} else {
		if (list->count == 1) {
			//printf("End of list!\n");
			
			list->head = NULL;
			list->tail = NULL;
		} else {
			if (listItem == list->tail) {
				//printf("Deleting tail! (C=%i)\n", list->count);
				
				listItem->prev->next = NULL;
				list->tail = listItem->prev;
			}
			
			if (listItem == list->head) {
				//printf("Deleting head! (C=%i)\n", list->count);
				
				listItem->next->prev = NULL;
				list->head = listItem->next;
			}
		}
	}
	
	free(listItem);

	-- list->count;
}

void _deleteListItem(linkedList_t *list, listItem_t *listItem) {
	list->deleteCallback(listItem->item);
	
	free(listItem->item);
}

void addListItem(linkedList_t *list, void *item) {
	listItem_t *listIterator = list->head, *newListItem;
	
	newListItem = malloc(sizeof(listItem_t));
	newListItem->prev = NULL;
	newListItem->next = NULL;

	if (newListItem == NULL) {
		puts("**FATAL** Could not create linked list item.\n");

		assert(1 == 2);
	}

	newListItem->item = item;

	list->tail = newListItem;

	if (!list->count) {
		list->head = newListItem;
	} else {
		while (listIterator->next) {
			listIterator = listIterator->next;
		}

		listIterator->next = newListItem;
		newListItem->prev = listIterator;
	}

	++ list->count;
}

void deleteListItem(linkedList_t *list, void *item) {
	listItem_t *listIterator = list->head;

	while (listIterator) {
		if (listIterator->item == item) {
			_deleteListItem(list, listIterator);
			_removeListItem(list, listIterator);

			return;
		}

		listIterator = listIterator->next;
	}

	assert(1 == 2); //Confuse the user
}

void clearList(linkedList_t *list) {
	//while (list->count) {
	//	popListItem(list);
	//}
	for (int i = list->count - 1; i >= 0; -- i)
		deleteListItem(list, getListItem(list, i));
}

void *getListItem(linkedList_t *list, int index) {
	listItem_t *listItem = list->head;

	assert(list->count > 0 && index < list->count);

	for (int i = 0; i < index; ++ i) {
		listItem = listItem->next;
	}

	return listItem->item;
}

void *popListItem(linkedList_t *list) {
	listItem_t *listIterator = list->head;
	void *itemPtr;

	while (listIterator->next) {
		listIterator = listIterator->next;
	}

	itemPtr = listIterator->item;
	_removeListItem(list, listIterator);

	return itemPtr;
}
