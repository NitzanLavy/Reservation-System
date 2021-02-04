#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list_mtm/list_mtm1.h"
#include "test_utilities.h"

static ListElement copyString(ListElement str){
	assert(str);
	char* copy = malloc(strlen(str)+1);
	return copy != NULL ? strcpy(copy, str) : NULL;
}

static bool isLetterInString(ListElement str, ListFilterKey key){
	if ( str == NULL ){
		return false;
	}
	char* string = (char*)str;
	char* letter = (char*)key;
	while ( *(string) ){
		if ( *(string) == *(letter) ){
			return true;
		}
		string++;
	}
	return false;
}

static int compareStrings(ListElement str1, ListElement str2){
	return strcmp((char*)str1, (char*)str2);
}

static void freeString(ListElement str){
	free(str);
}
static bool testListCreate(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(list!=NULL);
	listDestroy(list);
	list = listCreate(NULL, NULL);
	ASSERT_TEST(list==NULL);
	listDestroy(list);
	list = listCreate(NULL, freeString);
	ASSERT_TEST(list==NULL);
	listDestroy(list);
	list = listCreate(copyString, NULL);
	ASSERT_TEST(list==NULL);
	listDestroy(list);
	return true;
}

static bool testListCopy(){
	List list = listCreate(copyString, freeString);
	ListResult res = listInsertFirst(list, "kitty");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "cat");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "meow");
	ASSERT_TEST(res == LIST_SUCCESS);
	List copy_list;
	copy_list = listCopy(list);
	ASSERT_TEST(copy_list != NULL);
	ASSERT_TEST(strcmp("meow", listGetFirst(copy_list))==0);
	ASSERT_TEST(strcmp("cat", listGetNext(copy_list))==0);
	ASSERT_TEST(strcmp("kitty", listGetNext(copy_list))==0);
	ASSERT_TEST(listGetNext(copy_list)==NULL);
	listDestroy(copy_list);
	copy_list = listCopy(NULL);
	ASSERT_TEST(copy_list == NULL);
	listDestroy(copy_list);
	listDestroy(list);
	return true;
}

static bool testListFilter(){
	List list = listCreate(copyString, freeString);
	ListResult res = listInsertFirst(list, "AA");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "BAA");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "GEEEE");
	ASSERT_TEST(res == LIST_SUCCESS);
	char letter='A';
	ListFilterKey key = (void*)&letter;
	List list_a = listFilter(list, isLetterInString, key);
	ASSERT_TEST(list_a != NULL);
	ASSERT_TEST(listGetSize(list_a)==2);
	ASSERT_TEST(strcmp(listGetFirst(list_a), "BAA")==0);
	ASSERT_TEST(strcmp(listGetNext(list_a), "AA")==0);
	ASSERT_TEST(listFilter(NULL, isLetterInString, key)==NULL);
	ASSERT_TEST(listFilter(list, NULL, key)==NULL);
	listDestroy(list_a);
	listDestroy(list);
	return true;
}

static bool testListGetSize(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listGetSize(list)==0);
	ASSERT_TEST(listGetSize(NULL)==-1);
	ListResult res = listInsertFirst(list, "first");
	ASSERT_TEST(res == LIST_SUCCESS && listGetSize(list)==1);
	res = listInsertFirst(list, "second");
	ASSERT_TEST(res == LIST_SUCCESS && listGetSize(list)==2);
	listDestroy(list);
	return true;
}

static bool testListGetFirst(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listGetFirst(list)==NULL);
	ListResult res = listInsertFirst(list, "I am first!");
	ASSERT_TEST(res==LIST_SUCCESS &&
				strcmp(listGetFirst(list),"I am first!")==0);
	res = listInsertFirst(list,"You're second");
	ASSERT_TEST(res==LIST_SUCCESS
				&& strcmp(listGetFirst(list), "You're second")==0 );
	listDestroy(list);
	ASSERT_TEST(listGetFirst(NULL)==NULL);
	return true;
}

static bool testListGetNext(){
	List list = listCreate(copyString, freeString);
	ListResult res = listInsertFirst(list, "woo");
	ASSERT_TEST(res == LIST_SUCCESS);
	char* string = listGetFirst(list);
	ASSERT_TEST(listGetNext(list) == NULL);
	res = listInsertLast(list, "oow");
	ASSERT_TEST(res == LIST_SUCCESS);
	string = listGetFirst(list);
	string = listGetNext(list);
	ASSERT_TEST(string!=NULL && strcmp(string, "oow")==0);
	ASSERT_TEST(listGetNext(NULL)==NULL);
	listDestroy(list);
	return true;
}

static bool testListGetCurrent(){
	List list = listCreate(copyString, freeString);
	ListResult res = listInsertFirst(list, "first");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertLast(list, "last");
	char* string = listGetFirst(list);
	string = listGetCurrent(list);
	ASSERT_TEST(strcmp(string, "first")==0);
	string = listGetNext(list);
	string = listGetCurrent(list);
	ASSERT_TEST(strcmp(string,"last")==0);
	res = listRemoveCurrent(list);
	ASSERT_TEST(res == LIST_SUCCESS);
	ASSERT_TEST(listGetCurrent(list) == NULL);
	ASSERT_TEST(listGetCurrent(NULL)==NULL);
	listDestroy(list);
	return true;
}

static bool testListInsertFirst(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(list!=NULL);
	ListResult res = listInsertFirst(list, "first");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(NULL, "second");
	ASSERT_TEST(res == LIST_NULL_ARGUMENT);
	char* first = listGetFirst(list);
	ASSERT_TEST(strcmp(first, "first")==0);
	listDestroy(list);
	return true;
}

static bool testListInsertLast(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(list!=NULL); // no memory problem
	ListResult res = listInsertLast(list, "I'm last");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertLast(list, "Nah you're first");
	ASSERT_TEST(res == LIST_SUCCESS);
	char* first = listGetFirst(list);
	first = listGetNext(list);
	ASSERT_TEST(strcmp(first, "Nah you're first")==0);
	listDestroy(list);
	return true;
}

static bool testListInsertBeforeCurrent(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(list!=NULL);
	ListResult res = listInsertFirst(list, "tinky winky");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "lala");
	ASSERT_TEST(res == LIST_SUCCESS);
	char* string = listGetFirst(list);
	ASSERT_TEST(string!=NULL);
	string = listGetNext(list);
	ASSERT_TEST(string!=NULL);
	res = listInsertBeforeCurrent(list,"dipsy");
	ASSERT_TEST(res == LIST_SUCCESS &&
				strcmp(listGetCurrent(list),"dipsy")==0);
	res = listRemoveCurrent(list);
	ASSERT_TEST(listInsertBeforeCurrent(list, "oops") == LIST_INVALID_CURRENT);
	ASSERT_TEST(listInsertBeforeCurrent(NULL, "ooo") == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(list!=NULL);
	ListResult res = listInsertFirst(list, "matam");
	ASSERT_TEST(res == LIST_SUCCESS);
	char* str_first = listGetFirst(list);
	ASSERT_TEST(str_first != NULL);
	res = listInsertAfterCurrent(list, "atam");
	ASSERT_TEST(res == LIST_SUCCESS);
	char *first, *second;
	first = listGetFirst(list);
	ASSERT_TEST(strcmp(first,"matam")==0);
	second = listGetNext(list);
	ASSERT_TEST(strcmp(second,"atam")==0);
	ASSERT_TEST(listInsertAfterCurrent(NULL, "whoop")==LIST_NULL_ARGUMENT);
	res = listRemoveCurrent(list);
	ASSERT_TEST(res == LIST_SUCCESS
			&& listInsertAfterCurrent(list, "bla") == LIST_INVALID_CURRENT);
	listDestroy(list);
	return true;
}

static bool testListRemoveCurrent(){
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
	ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
	ListResult res = listInsertFirst(list, "abc");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "123");
	ASSERT_TEST(res == LIST_SUCCESS);
	char* str = listGetFirst(list);
	ASSERT_TEST(str!=NULL);
	res = listRemoveCurrent(list);
	ASSERT_TEST(res == LIST_SUCCESS &&
				strcmp(listGetFirst(list),"abc") == 0);
	listDestroy(list);
	return true;
}

static bool testListSort(){
	List list = listCreate(copyString, freeString);
	ListResult res = listInsertFirst(list, "aaa");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "bbb");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listInsertFirst(list, "abb");
	ASSERT_TEST(res == LIST_SUCCESS);
	res = listSort(list, compareStrings);
	ASSERT_TEST(res == LIST_SUCCESS);
	char* str = listGetFirst(list);
	ASSERT_TEST(str!=NULL &&strcmp(str,"aaa")==0);
	str = listGetNext(list);
	ASSERT_TEST(str!=NULL && strcmp(str,"abb")==0);
	str = listGetNext(list);
	ASSERT_TEST(str!=NULL && strcmp(str,"bbb")==0);
	ASSERT_TEST(listSort(NULL,NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(NULL, compareStrings)==LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(list, NULL)==LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}

static bool testListClear(){
	List list = listCreate(copyString, freeString);
	ListResult res = listInsertFirst(list, "insert");
	ListResult res2 = listInsertFirst(list, "insert2");
	ASSERT_TEST(res == LIST_SUCCESS && res2 == LIST_SUCCESS &&
				listGetFirst(list)!=NULL);
	ListResult res3 = listClear(list);
	ASSERT_TEST(res3 == LIST_SUCCESS && listGetFirst(list) == NULL );
	listDestroy(list);
	ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
	return true;
}



int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListFilter);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListGetCurrent);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	return 0;
}
