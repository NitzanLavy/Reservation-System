

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


#include "order.h"
#include "escaper.h"
#include "list.h"
#include "escaper_input_checks.h"

struct SEscaper {
	char *email;
	int faculty;
	int skill_level;
	List orders;
};


EscaperResult translateOrderEscaper(OrderResult order_result) {
	switch (order_result) {
		case ORDER_OUT_OF_MEMORY:
			return ESCAPER_OUT_OF_MEMORY;
		case ORDER_NULL_PARAMETER:
			return ESCAPER_NULL_PARAMETER;
		case ORDER_INVALID_PARAMETER:
			return ESCAPER_INVALID_PARAMETER;
		case ORDER_SUCCESS:
			return ESCAPER_SUCCESS;
		default: return -1;
	}
}

EscaperResult translateListEscaper(ListResult list_result) {
	switch (list_result) {
		case LIST_OUT_OF_MEMORY:
			return ESCAPER_OUT_OF_MEMORY;
		case LIST_NULL_ARGUMENT:
			return ESCAPER_NULL_PARAMETER;
		case LIST_INVALID_CURRENT:
			return ESCAPER_INVALID_PARAMETER;
		case LIST_SUCCESS:
			return ESCAPER_SUCCESS;
		default: return -1;
	}
}


EscaperResult isEscaperInRoom(Escaper escaper, int hour, int day,
									 bool* result){
	if ( escaper == NULL || result == NULL ){
		return ESCAPER_NULL_PARAMETER;
	}
	if ( !isLegalOrderHour(hour) || !isLegalDay(day) ){
		return ESCAPER_INVALID_PARAMETER;
	}
	Order curr_order = listGetFirst(escaper->orders);
	int curr_hour, curr_day;
	OrderResult order_res;
	while ( curr_order != NULL ){
		order_res = orderGetHour(curr_order, &curr_hour);
		if ( order_res != ORDER_SUCCESS ){
			return translateOrderEscaper(order_res);
		}
		order_res = orderGetDay(curr_order, &curr_day);
		if ( order_res != ORDER_SUCCESS ){
			return translateOrderEscaper(order_res);
		}
		if ( hour == curr_hour && day == curr_day ){
			*result = true;
			return ESCAPER_SUCCESS;
		}
		curr_order = listGetNext(escaper->orders);
	}
	*result = false;
	return ESCAPER_SUCCESS;
}


/*
 * Generic copy function of orders designed for use for generic lists.
 * The function uses the regular orderCopy function.
 * The function returns a copy of a given order element.
 */
static ListElement orderCopyForList(ListElement order){
	Order original_order = (Order)order;
	OrderResult result;
	Order order_copy = orderCopy(original_order, &result);
	EscaperResult res = translateOrderEscaper(result);
	if (res != ESCAPER_SUCCESS) {
		return NULL;
	}
	return (ListElement)order_copy;
}

/*
 * Generic destroy function of orders designed for use for generic lists.
 * The function uses the regular orderDestroy function.
 * The function destroys a given order element.
 */
static void orderDestroyForList(ListElement order){
	orderDestroy((Order)order);
}


Escaper escaperCreate (char *email, int faculty, int skill_level,
		EscaperResult *result) {
	assert(result!=NULL);
	if ( email == NULL ){
		*result = ESCAPER_NULL_PARAMETER;
		return NULL;
	}
	if (!isLegalEmail(email)|| !isLegalSkillLevel(skill_level) ||
		!isLegalFaculty(faculty) ) {
		*result =  ESCAPER_INVALID_PARAMETER;
		return NULL;
	}
	Escaper escaper = malloc(sizeof(*escaper));
	if (escaper == NULL) {
		*result =  ESCAPER_OUT_OF_MEMORY;
		return NULL;
	}
	char *new_email = malloc (strlen(email)+1);
	if (new_email == NULL) {
		*result =  ESCAPER_OUT_OF_MEMORY;
		free(escaper);
		return NULL;
	}
	strcpy(new_email,email);
	escaper->email = new_email;
	escaper->skill_level = skill_level;
	escaper->faculty = faculty;
	escaper->orders = listCreate(orderCopyForList,orderDestroyForList);
	if ( escaper->orders == NULL ){
		free(escaper->email);
		free(escaper);
		*result = ESCAPER_OUT_OF_MEMORY;
		return NULL;
	}
	*result =  ESCAPER_SUCCESS;
	return escaper;
}


Escaper escaperCopy (Escaper escaper, EscaperResult *result) {
	assert (escaper != NULL);
	Escaper new_escaper = malloc (sizeof(*new_escaper));
	if (new_escaper == NULL) {
		*result = ESCAPER_OUT_OF_MEMORY;
		return NULL;
	}
	char *new_email = malloc(strlen(escaper->email)+1);
	if (new_email == NULL) {
		*result = ESCAPER_OUT_OF_MEMORY;
		escaperDestroy (new_escaper);
		return NULL;
	}
	strcpy (new_email,escaper->email);
	new_escaper->email = new_email;
	new_escaper->skill_level = escaper->skill_level;
	new_escaper->faculty = escaper->faculty;
	new_escaper->orders = listCopy(escaper->orders);
	if ( new_escaper->orders == NULL ){
		free(new_escaper->email);
		free(new_escaper);
		*result = ESCAPER_OUT_OF_MEMORY;
		return NULL;
	}
	*result = ESCAPER_SUCCESS;
	return new_escaper;
}


void escaperDestroy (Escaper escaper) {
	if (escaper == NULL) {
		return;
	}
	free(escaper->email);
	escaper->email = NULL;
	listDestroy(escaper->orders);
	escaper->orders = NULL;
	free(escaper);
}

int escaperCompare (Escaper escaper1, Escaper escaper2) {
	assert (escaper1 != NULL && escaper2 != NULL);
	return (strcmp(escaper1->email,escaper2->email));
}


EscaperResult escaperGetEmail(Escaper escaper, char** email_copy) {
	if (escaper == NULL || email_copy == NULL) {
		return ESCAPER_NULL_PARAMETER;
	}
	assert(escaper->email!=NULL);
	*email_copy = malloc(strlen(escaper->email)+1);
	if (*email_copy == NULL) {
		return ESCAPER_OUT_OF_MEMORY;
	}
	strcpy(*email_copy,escaper->email);
	return ESCAPER_SUCCESS;
}

EscaperResult escaperGetFaculty(Escaper escaper, int* faculty){
	if (escaper == NULL || faculty == NULL) {
		return ESCAPER_NULL_PARAMETER;
	}
	*faculty = escaper->faculty;
	return ESCAPER_SUCCESS;
}

EscaperResult escaperGetSkillLevel(Escaper escaper, int* skill_level){
	if (escaper == NULL || skill_level == NULL) {
		return ESCAPER_NULL_PARAMETER;
	}
	*skill_level = escaper->skill_level;
	return ESCAPER_SUCCESS;
}

EscaperResult escaperAddOrder (Escaper escaper, int faculty, int id, int day,
							   int hour, int num_ppl) {
	if (escaper == NULL ) {
		return ESCAPER_NULL_PARAMETER;
	}
	assert(escaper->orders!=NULL);
	OrderResult order_res;
	if ( !isLegalFaculty(faculty) || !isLegalId(id) || !isLegalDay(day) ||
		 !isLegalOrderHour(hour) || !isLegalNumPpl(num_ppl) ){
		return ESCAPER_INVALID_PARAMETER;
	}
	Order new_order = orderCreate(faculty, id, day, hour, num_ppl, &order_res);
	EscaperResult escaper_res = translateOrderEscaper(order_res);
	if (escaper_res != ESCAPER_SUCCESS) {
		return escaper_res;
	}
	bool isInRoom;
	escaper_res = isEscaperInRoom(escaper, hour, day, &isInRoom);
	if ( isInRoom ){
		orderDestroy(new_order);
		return ESCAPER_CLIENT_IN_ROOM;
	}
	ListResult list_res = listInsertLast(escaper->orders, new_order);
	escaper_res = translateListEscaper(list_res);
	orderDestroy(new_order);
	return escaper_res;
}

EscaperResult escaperNextDay(Escaper escaper){
	if ( escaper == NULL ){
		return ESCAPER_NULL_PARAMETER;
	}
	Order order = listGetFirst(escaper->orders);
	while ( order!=NULL ){
		orderDecDay(order);
		order = listGetNext(escaper->orders);
	}
	return ESCAPER_SUCCESS;
}

EscaperResult escaperMaximalRoomId(Escaper escaper, int* maximal_id){
	if ( escaper == NULL || maximal_id == NULL ){
		return ESCAPER_NULL_PARAMETER;
	}
	Order order = listGetFirst(escaper->orders);
	if ( order == NULL ){
		*maximal_id = MIN_ID-1;
		return ESCAPER_SUCCESS;
	}
	int max, current;
	orderGetRoomId(order, &max);
	while ( order!= NULL ){
		orderGetRoomId(order,&current);
		if ( current > max ){
			max = current;
		}
		order = listGetNext(escaper->orders);
	}
	*maximal_id = max;
	return ESCAPER_SUCCESS;
}

EscaperResult escaperRemoveOldOrders(Escaper escaper, int num_of_days){
	if ( escaper == NULL ){
		return ESCAPER_NULL_PARAMETER;
	}
	if ( !isLegalDay(num_of_days) ){
		return ESCAPER_INVALID_PARAMETER;
	}
	Order order = listGetFirst(escaper->orders);
	int order_day;
	while ( order!=NULL ){ // go through all orders
		OrderResult o_res = orderGetDay(order, &order_day);
		EscaperResult e_res = translateOrderEscaper(o_res);
		if ( e_res != ESCAPER_SUCCESS ){
			return e_res;
		}
		if ( order_day < num_of_days ){ // check if the current order's day
			// is less than the given number of days
			listRemoveCurrent(escaper->orders); // remove the old order
			order = listGetFirst(escaper->orders); // start scanning again
			continue;
		}
		order = listGetNext(escaper->orders);
	}
	return ESCAPER_SUCCESS;
}

EscaperResult searchMatchingOrder(Escaper escaper, int faculty,int id,
							      bool *found){
	if ( escaper == NULL || found == NULL ){
		return ESCAPER_NULL_PARAMETER;
	}
	if ( !isLegalFaculty(faculty) || !isLegalId(id) ){
		return ESCAPER_INVALID_PARAMETER;
	}
	Order order = listGetFirst(escaper->orders);
	int order_faculty, order_id;
	while ( order != NULL ){
		orderGetRoomId(order, &order_id);
		orderGetRoomFaculty(order, &order_faculty);
		if ( order_id == id && order_faculty == faculty ){
			*found = true;
			return ESCAPER_SUCCESS;
		}
		order = listGetNext(escaper->orders);
	}
	*found = false;
	return ESCAPER_SUCCESS;
}

EscaperResult escaperListOrders(Escaper escaper, List *list){
	if ( escaper == NULL || list == NULL ){
		return ESCAPER_NULL_PARAMETER;
	}
	if ( escaper->orders == NULL ){
		*list = NULL;
		return ESCAPER_SUCCESS;
	}
	List copy_list = listCopy(escaper->orders);
	if ( copy_list == NULL ){
		*list = NULL;
		return ESCAPER_OUT_OF_MEMORY;
	}
	*list = copy_list;
	return ESCAPER_SUCCESS;
}



