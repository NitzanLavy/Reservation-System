

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


#include "order.h"
#include "order_input_checks.h"

struct SOrder {
	int room_faculty;
	int room_id;
	int day;
	int hour;
	int num_ppl;
};

Order orderCreate (int faculty, int id, int day, int hour, int num_ppl,
		OrderResult *result) {
	assert (result != NULL);
	if (!isLegalOrderHour(hour) || !isLegalNumPpl(num_ppl) ||
			!isLegalDay(day) || !isLegalId(id) || !isLegalFaculty(faculty)) {
		*result = ORDER_INVALID_PARAMETER;
		return NULL;
	}
	Order new_order = malloc (sizeof(*new_order));
	if (new_order == NULL) {
		*result = ORDER_OUT_OF_MEMORY;
		return NULL;
	}
	new_order->room_faculty = faculty;
	new_order->room_id = id;
	new_order->day = day;
	new_order->hour = hour;
	new_order->num_ppl = num_ppl;
	*result = ORDER_SUCCESS;
	return new_order;
}


Order orderCopy (Order order, OrderResult *result) {
	assert(result!=NULL);
	if (order == NULL) {
		*result = ORDER_NULL_PARAMETER;
		return NULL;
	}
	Order new_order = orderCreate(order->room_faculty, order->room_id,
			order->day, order->hour, order->num_ppl, result);
	return new_order;
}


void orderDestroy(Order order) {
	if (order != NULL) {
		free(order);
	}
}


OrderResult orderDecDay (Order order) {
	if (order == NULL) {
		return ORDER_NULL_PARAMETER;
	}
	order->day = (order->day) -1;
	return ORDER_SUCCESS;
}


OrderResult orderGetDay (Order order, int *day) {
	if (order == NULL || day == NULL) {
		return ORDER_NULL_PARAMETER;
	}
	*day = order->day;
	return ORDER_SUCCESS;
}


OrderResult orderGetHour (Order order, int *hour) {
	if (order == NULL || hour == NULL) {
		return ORDER_NULL_PARAMETER;
	}
	*hour = order->hour;
	return ORDER_SUCCESS;
}


OrderResult orderGetNumPpl (Order order, int *num) {
	if (order == NULL || num == NULL) {
		return ORDER_NULL_PARAMETER;
	}
	*num = order->num_ppl;
	return ORDER_SUCCESS;
}

OrderResult orderGetRoomId (Order order, int *id){
	if (order == NULL || id == NULL) {
		return ORDER_NULL_PARAMETER;
	}
	*id = order->room_id;
	return ORDER_SUCCESS;
}

OrderResult orderGetRoomFaculty (Order order, int *faculty){
	if (order == NULL || faculty == NULL) {
		return ORDER_NULL_PARAMETER;
	}
	*faculty = order->room_faculty;
	return ORDER_SUCCESS;
}


