
#ifndef ESCAPER_H_
#define ESCAPER_H_

#include "order.h"
#include "stdbool.h"
#include "list.h"
typedef struct SEscaper* Escaper;

typedef enum {
	ESCAPER_OUT_OF_MEMORY,
	ESCAPER_NULL_PARAMETER,
	ESCAPER_INVALID_PARAMETER,
	ESCAPER_CLIENT_IN_ROOM,
	ESCAPER_SUCCESS
}EscaperResult;

/*
 * Converts error messages from OrdeResult type to EscaperResult type messages.
 */
EscaperResult translateOrderEscaper(OrderResult order_result);

/*
 * Converts error messages from ListResult type to EscaperResult type messages.
 */
EscaperResult translateListEscaper(ListResult list_result);

/*
 * Creates a new escaper.
 * Receives escaper's email, faculty and skill level.
 * returns NULL if there was a problem with memory allocation or a problem with
 * one of the input parameters. If everything went fine returns a pointer to
 * the new escaper.
 * result is updated accordingly.
 */
Escaper escaperCreate (char *email, int faculty, int skill_level,
		EscaperResult *result);

/*
 * Create a copy of an escaper.
 * Receives an escaper parameter.
 * returns NULL if there was a problem with memory allocation or a problem with
 * the input parameter. If everything went fine returns a pointer to
 * the escaper's copy.
 * result is updated accordingly.
 */
Escaper escaperCopy (Escaper escaper, EscaperResult *result);


/*
 * Deallocate an escaper and free all of it's internal memory.
 * Receives an escaper parameter.
 * If the received parameter is NULL nothing will be done.
 */
void escaperDestroy (Escaper escaper);


/*
 * Compares the email of the two received escaper parameters.
 * If the email of the two escapers is equal it return 0, if not it returns 1.
 * If one of the parameters is NULL it returns NULL.
 */
int escaperCompare (Escaper escaper1, Escaper escaper2);

/*
 * Returns a copy of the email of given escaper.
 * The function returns a message indicating it's success. if returned
 * ESCAPER_SUCCESS than a copy of escaper's email is returned in the parameter
 * email_copy, if not than an error occured and the return message is returned
 * accordingly.
 */
EscaperResult escaperGetEmail(Escaper escaper, char** email_copy);

/*
 * Returns the given escaper's faculty.
 * The function returns a message indicating it's success. if returned
 * ESCAPER_SUCCESS then the faculty was updated in the given parameter.
 * returns ESCAPER_NULL_PARAMETER if one of the parameters is NULL
 */
EscaperResult escaperGetFaculty(Escaper escaper, int* faculty);

/*
 * Returns the given escaper's skill level.
 * The functions returns a message indicating it's success. if returned
 * ESCAPER_SUCCESS then the skill level was updated in the given parameter.
 * return ESCAPER_NULL_PARAMETER if one of the parameters is NULL
 */
EscaperResult escaperGetSkillLevel(Escaper escaper, int* skill_level);

/*
 * Adds a new order to the orders list of escaper.
 * The function uses orderCreate to create a new order, and than add it to the
 * end of the orders list using listInsertLast function.
 * Returns ESCAPER_NULL_PARAMETER if escaper is NULL. Otherwise, return value
 * is based on success or failure of creating a new order and adding it to the
 * end of the list.
 */
EscaperResult escaperAddOrder (Escaper escaper, int faculty, int id, int day,
		int hour, int num_ppl);

///*
// * Prints the orders that have the same room id, room faculty, day and hour as
// * the given parameters.
// * returns ESCAPER_NULL_PARAMETER if the given escaper is NULL
// * returns ESCAPER_INVALID_PARAMETER if one of the given parameters is illegal
// * returns ESCAPER_SUCCESS otherwise
// */
//EscaperResult escaperPrintOrders(Escaper escaper, int room_id, int room_faculty,
//								int order_hour, int day);

/*
 * Moves to next day - the number of days left for each order is decreased by 1
 * returns ESCAPER_NULL_PARMAETER if the given escaper is NULL
 * returns ESCAPER_SUCCESS otherwise
 */
EscaperResult escaperNextDay(Escaper escaper);

/*
 * Receives an escaper a number of days. Removes all of escaper's orders that
 * have less days left than num_of_days.
 * returns ESCAPER_NULL_PARAMETER if escaper is NULL
 * returns ESCAPER_INVALID_PARAMETER if the number of days is illegal
 * returns ESCAPER_SUCCESS otherwise
 */
EscaperResult escaperRemoveOldOrders(Escaper escaper, int num_of_days);

/*
 * Returns maximal room id out of the given escaper's orders.
 *  maximal_id is set to (MIN_ID) - 1 if there are no orders
 * returns ESCAPER_NULL_PARAMETER if one of the given parameters is NULL
 * returns ESCAPER_SUCCESS otherwise
 */
EscaperResult escaperMaximalRoomId(Escaper escaper, int* maximal_id);

/* updates found to be true if escaper has an order that has the same faculty
 * and id as the parameters given. found is updated to false otherwise, and
 * returns ESCAPER_SUCCESS. if one of the given parameter is NULL, in which
 * case returns ESCAPER_NULL_PARAMETER. if the faculty/id is illegal
 * return ESCPAER_INVALID_PARAMETER
 */
EscaperResult searchMatchingOrder(Escaper escaper, int faculty,int id,
							      bool *found);
/* returns ESCAPER_NULL_PARAMETER if one of the parameters is NULL
 * returns ESCAPER_OUT_OF_MEMORY if there were memory, and list is set to NULL
 * otherwise, returns ESCAPER_SUCCESS and assigns list to a copy of a list
 * containing all of the given escaper's orders
 */
EscaperResult escaperListOrders(Escaper escaper, List* list);

/* receives an escaper, an hour, a day and a pointer to a bool type varibale
 * returns ESCAPER_NULL_PARAMETER if one of the parameters is NULL
 * returns ESCAPER_INVALID_PARAMETER if one of the parameters is illegal
 * otherwise updates result to true if the given escaper is in a room in the
 * given day and hour and to false if not
 */
EscaperResult isEscaperInRoom(Escaper escaper, int hour, int day,
									 bool* result);


#endif /* ESCAPER_H_ */
