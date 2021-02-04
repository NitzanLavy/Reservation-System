
#ifndef ORDER_H_
#define ORDER_H_

#include "general_input_checks.h"

typedef struct SOrder* Order;

typedef enum {
	ORDER_OUT_OF_MEMORY,
	ORDER_NULL_PARAMETER,
	ORDER_INVALID_PARAMETER,
	ORDER_SUCCESS
}OrderResult;


/*
 * Creates a new order.
 * Receives a pointer to escaper, faculty, room id, day and hour parameters,
 * and number of people.
 * returns NULL if there was a problem with memory allocation or a problem with
 * one of the input parameters. If everything went fine returns a pointer to
 * the new order.
 * result is updated accordingly.
 */
Order orderCreate (int room_faculty, int room_id, int day, int hour,
		int num_ppl, OrderResult *result);


/*
 * Create a copy of an order.
 * Receives an order parameter.
 * returns NULL if there was a problem with memory allocation or a problem with
 * the input parameter. If everything went fine returns a pointer to
 * the order's copy.
 * result is updated accordingly.
 */
Order orderCopy (Order order, OrderResult *result);

/*
 * Deallocate an order and free all of it's internal memory.
 * Receives an order parameter.
 * If the received parameter is NULL nothing will be done.
 */
void orderDestroy(Order order);

/*
 * Receives an order parameter, and decreases its day by 1.
 * If there was no problem with the given parameter returns ORDER_SUCCESS.
 */
OrderResult orderDecDay (Order order);

/*
 * Receives an order parameter, and returns it's day number.
 */
OrderResult orderGetDay (Order order, int *day);

/*
 * Receives an order parameter, and returns it's hour number.
 */
OrderResult orderGetHour (Order order, int *hour);

/*
 * Receives an order parameter, and returns it's people number.
 */
OrderResult orderGetNumPpl (Order order, int *num);

/*
 * Receives an order parameter, and returns it's room id.
 */
OrderResult orderGetRoomId (Order order, int *id);

/*
 * Receives an order parameter, and returns it's room's faculty.
 */
OrderResult orderGetRoomFaculty (Order order, int *faculty);

#endif /* ORDER_H_ */
