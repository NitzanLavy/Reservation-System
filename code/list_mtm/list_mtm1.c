
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "list_mtm1.h"

struct list_t {
	struct SElementList* header;
	struct SElementList* iterator;
	CopyListElement copyElement;
	FreeListElement freeElement;
	int size;
	struct SElementList {
		ListElement element;
		struct SElementList* next;
	}SElementList;
};

/*
 * Receives a list and number n.
 * The function returns the SElementList in n place in list.
 */
static struct SElementList* findInPLace (List list, int n);

/*
 * Receives a list and SElementList parameter.
 * The function finds and returns the SElementList before the given SElementList
 * in the list.
 */
static struct SElementList* findBefore (List list,
		struct SElementList* elementlist);

/*
 * Receives a list and two SElementList parameters.
 * The function swap the places of the two SElementList in list.
 */
static void swap (List list, struct SElementList* elementlist1,
		struct SElementList* elementlist2);

/*
 * Receives a list, a pointer to comparison function and number.
 * The function uses bubble sort algorithm to sort the given list.
 */
static int bubbleSort (List list, CompareListElements compareElement, int n,
		ListResult* res);

/*
 * Receives a list.
 * The function returns the number of the node in the list that the list's
 * iterator is pointing at.
 */
static int iteratorCounter(List list);

/*
 * Receives a list and a number.
 * The function initializes the list's iterator to point at the node in the
 * number position.
 */
static void initIterator (List list, int counter);

/*
 * Receives a list and an element.
 * The function creates and returns a new SElementList that contains a copy
 * of given element. field next of SElementList is not initialized.
 */
static struct SElementList* createElementList (List list, ListElement element);

/*
 * Receives two lists.
 * The function is being used by listCopy function to add all of the elements
 * in toAdd list to the given list. If the function fails, list is destroyed.
 */
static ListResult addAllOrDestroy (List list, List toAdd);



static struct SElementList* findInPLace (List list, int n) {
	struct SElementList* current = list->header;
	while (n>0) {
		n--;
		current = current->next;
	}
	return current;
}

static struct SElementList* findBefore (List list,
		struct SElementList* elementlist) {
	struct SElementList* current = list->header;
	if ( current == elementlist ){
		return NULL;
	}
	while (current->next != elementlist) {
		current = current->next;
	}
	return current;
}

static void swap (List list, struct SElementList* elementlist1,
		struct SElementList* elementlist2) {
	struct SElementList* before1 = findBefore(list, elementlist1);
	elementlist1->next = elementlist2->next;
	if ( before1 != NULL ){
		before1->next = elementlist2;
	}
	elementlist2->next = elementlist1;
	if ( before1 == NULL ){
		list->header = elementlist2;
	}
}

static int bubbleSort (List list, CompareListElements compareElement, int n,
		ListResult* res) {
	int i, is_swapped = 0;
	for (i=1;i<n;i++) {
		struct SElementList* elementlist1 = findInPLace(list,i-1);
		struct SElementList* elementlist2 = findInPLace(list,i);
		if (compareElement(elementlist1->element,elementlist2->element) > 0 ) {
			swap(list, elementlist1,elementlist2);
			is_swapped = 1;
		}
	}
	*res = LIST_SUCCESS;
	return is_swapped;
}

static int iteratorCounter(List list) {
	struct SElementList* current = list->header;
	int counter = 0;
	while (current != list->iterator) {
		counter++;
		current = current->next;
	}
	return counter;
}

static void initIterator (List list, int counter) {
	struct SElementList* current = list->header;
	while (counter > 1) {
		current = current->next;
		counter--;
	}
	list->iterator = current;
}

static struct SElementList* createElementList (List list, ListElement element){
	if (list == NULL || element == NULL) {
		return NULL;
	}
	ListElement new_element = list->copyElement(element);
	if (new_element == NULL) {
		return NULL;
	}
	struct SElementList* new_elementlist = malloc(sizeof(struct SElementList));
	if (new_elementlist == NULL) {
		free (new_element);
		return NULL;
	}
	new_elementlist->element = new_element;
	return new_elementlist;
}


static ListResult addAllOrDestroy (List list, List toAdd) {
	struct SElementList* current = toAdd->header;
	ListResult res;
	while (current!=NULL) {
		ListElement new_element = list->copyElement(current->element);
		if (new_element == NULL) {
			listDestroy(list);
			return LIST_OUT_OF_MEMORY;
		}
		res = listInsertLast(list,new_element);
		free(new_element); // added 176-180 (insertLast creates a copy of
		// new_element)
		if ( res == LIST_OUT_OF_MEMORY ){ //insertLast might have failed copying
			listDestroy(list);
			return LIST_OUT_OF_MEMORY;
		}
		current = current->next;
	}
	return LIST_SUCCESS;
}



List listCreate (CopyListElement copyElement, FreeListElement freeElement) {
	if (copyElement == NULL || freeElement == NULL) {
		return NULL;
	}
	List list = malloc (sizeof(*list));
	if (list == NULL) {
		return NULL;
	}
	list->header = NULL;
	list->iterator = NULL;
	list->copyElement = copyElement;
	list->freeElement = freeElement;
	list->size = 0;
	return list;
}

List listCopy (List list) {
	if (list == NULL) {
		return NULL;
	}
	List new_list = listCreate (list->copyElement, list->freeElement);
	if (new_list == NULL) {
		return NULL;
	}
	if (addAllOrDestroy(new_list, list) == LIST_OUT_OF_MEMORY) {
		//free (new_list); // list already destroyed in addAllOrDestroy
		return NULL;
	}
	int counter = iteratorCounter(list);
	initIterator(new_list,counter);
	return new_list;
}


int listGetSize(List list) {
	if (list == NULL) {
		return -1;
	}
	return list->size;
}



ListElement listGetFirst(List list) {
	if (list == NULL ) {
		return NULL;
	}
	list->iterator = list->header;
	if ( list->iterator == NULL ){
		return NULL;
	}
	return list->iterator->element;
}



ListElement listGetNext(List list) {
	if(list == NULL || list->iterator == NULL || list->iterator->next == NULL){
			return NULL;
	}
	list->iterator = list->iterator->next;
	return list->iterator->element;
}



ListElement listGetCurrent(List list) {
	if(list == NULL || list->iterator == NULL){
			return NULL;
	}
	return list->iterator->element;
}



ListResult listInsertFirst(List list, ListElement element) {
	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	struct SElementList* new_elementlist = createElementList(list,element);
	if (new_elementlist == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
	new_elementlist->next = list->header;
	list->header = new_elementlist;
	list->size++;
	return LIST_SUCCESS;
}



ListResult listInsertLast(List list, ListElement element) {
	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	struct SElementList* new_elementlist = createElementList(list,element);
	if (new_elementlist == NULL) {
		return LIST_OUT_OF_MEMORY;
	}

	struct SElementList* current = list->header;
	if (current == NULL) {
		list->header = new_elementlist;
	}
	else {
		while (current->next != NULL) {
			current=current->next;
		}
		current->next = new_elementlist;
	}

	new_elementlist->next = NULL;
	list->size++;
	return LIST_SUCCESS;
}



ListResult listInsertBeforeCurrent(List list, ListElement element) {
	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	if (list->iterator == NULL && list->header!=NULL) {
		return LIST_INVALID_CURRENT;
	}
	if ( list->header == list->iterator ){
		ListResult res = listInsertFirst(list, element);
		list->iterator = list->header;
		return res;
	}
	struct SElementList* new_elementlist = createElementList(list,element);
	if (new_elementlist == NULL) {
		return LIST_OUT_OF_MEMORY;
	}
	struct SElementList* current = list->header;
	while (current->next != list->iterator) {
		current = current->next;
	}
	current->next = new_elementlist;
	new_elementlist->next = list->iterator;
	list->iterator = new_elementlist;
	list->size++;
	return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	if (list->iterator == NULL) {
		return LIST_INVALID_CURRENT;
	}
	struct SElementList* new_elementlist = createElementList(list,element);
	if (new_elementlist == NULL) {
		return LIST_OUT_OF_MEMORY;
	}

	new_elementlist->next = list->iterator->next;
	list->iterator->next = new_elementlist;
	list->size++;
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	if (list->iterator == NULL) {
		return LIST_INVALID_CURRENT;
	}
	list->freeElement(list->iterator->element);
	struct SElementList* current = list->header;
	if ( current == list->iterator ){
		list->header = current->next;
		free(current);
		return LIST_SUCCESS;
	}
	while (current->next != list->iterator) {
		current = current->next;
	}
	current->next = list->iterator->next;
	free (list->iterator);
	list->iterator = NULL;
	list->size--;
	return LIST_SUCCESS;
}


ListResult listSort(List list, CompareListElements compareElement) {
	if (list == NULL || compareElement == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	int counter = iteratorCounter (list);
	int not_sorted = 1, n = list->size;
	ListResult res;
	while (n>1 && not_sorted) {
		not_sorted = bubbleSort(list, compareElement, n--, &res);
		if (res != LIST_SUCCESS) {
			return res;
		}
	}
	initIterator(list,counter);
	return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
	if (list == NULL || filterElement == NULL) {
		return NULL;
	}
	List new_list = listCreate(list->copyElement,list->freeElement);
	if ( new_list == NULL ){
		return NULL;
	}
	ListResult res;
	LIST_FOREACH(struct SElementList*,iterator,list) {
		if (filterElement(listGetCurrent(list),key)) {
			res = listInsertLast(new_list,listGetCurrent(list));
			if (res != LIST_SUCCESS) {
				listDestroy(new_list);
				return NULL;
			}
		}
	}
	new_list->iterator = new_list->header;
	return new_list;
}

ListResult listClear(List list) {
	if (list == NULL) {
		return LIST_NULL_ARGUMENT;
	}
	LIST_FOREACH(struct SElementList*,iterator,list) {
		ListResult res = listRemoveCurrent(list);
		if ( res != LIST_SUCCESS ){
			return res;
		}
		list->iterator = list->header;
	}
	if (list->header != NULL) {
		list->freeElement(list->header->element);
		free(list->header);
	}
	list->header = NULL;
	return LIST_SUCCESS;
}

void listDestroy(List list) {
	if (listClear(list) == LIST_SUCCESS) {
		free(list);
		list=NULL;
	}
}


