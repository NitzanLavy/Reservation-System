
#ifndef ESCAPETECHNION_H_
#define ESCAPETECHNION_H_

typedef struct s_escapeTechnion* EscapeTechnion;
#include "mtm_ex3.h"

typedef enum {
	ET_OUT_OF_MEMORY,
	ET_NULL_PARAMETER,
	ET_INVALID_PARAMETER,
	ET_EMAIL_ALREADY_EXISTS,
	ET_COMPANY_EMAIL_DOES_NOT_EXIST,
	ET_CLIENT_EMAIL_DOES_NOT_EXIST,
	ET_ID_ALREADY_EXIST,
	ET_ID_DOES_NOT_EXIST,
	ET_CLIENT_IN_ROOM,
	ET_ROOM_NOT_AVAILABLE,
	ET_RESERVATION_EXIST,
	ET_NO_ROOMS_AVAILABLE,
	ET_SUCCESS
}EscapeTechnionResult;

/* creates a new EscapeTechnion.
 * updates result to ET_OUT_OF_MEMORY if it could not allocate the memory needed
 * and returns NULL.
 * updates result to ET_SUCCESS and returns the new EscapeTechnion otherwise. */
EscapeTechnion addEscapeTechnion (EscapeTechnionResult *result);

/* receives an existing EscapeTechnion and returns a copy of it.
 * returns NULL if there were memory problems or if escapeTechnion is NULL
 * result is updated to ET_OUT_OF_MEMORY in case of memory problems
 * result is updated to ET_NULL_PARAMETER if escapeTechnion is NULL
 * result is updated to ET_SUCCESS otherwise
 */
EscapeTechnion copyEscapeTechnion (EscapeTechnion escapeTechnion,
								   EscapeTechnionResult *result);

/* frees all of the allocated memory in the given escapeTechnion.
 * does nothing if escapeTechnion is NULL */
void destroyEscapeTechnion (EscapeTechnion escapeTechnion);

/* receives escapeTechnion and adds a company with the given details.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_OUT_OF_MEMORY in case of memory allocation problems
 * returns ET_INVALID_PARAMETER if the faculty/email is illegal
 * returns ET_EMAIL_ALREADY_EXIST if the company email is already in the system
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult addCompany (EscapeTechnion escapeTechnion, char* email,
								 int faculty);
/* receives escapeTechnion and a company email and removed the given company
 * from the system, freeing all of its allocations. fails to remove the company
 * if the company has future orders
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_INVALID_PARAMETER if the email is illegal
 * returns ET_RESERVATION_EXIST if the company has future orders
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult removeCompany (EscapeTechnion escapeTechnion, char *email);

/* receives escapeTechnion, an email, a room id, price and recommended number
 * of people. adds the room to the given company with the given details.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_OUT_OF_MEMORY in case of memory allocation problems
 * returns ET_INVALID_PARAMETER if received an illegal parameter
 * returns ET_ID_ALREADY_EXIST if the room id already exists in the faculty
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult addRoom (EscapeTechnion escapeTechnion, char *email,
							  int id, int price, int rec_num_ppl,
							  int open_hour, int close_hour, int difficulty);
/* receives escapeTechnion, a faculty and a room id. removes the room from the
 * system and frees all of its allocated memory. fails to remove the room if it
 * has future orders.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_INVALID_PARAMETER one of the parameters is illegal
 * returns ET_RESERVATION_EXIST if the company has future orders
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult removeRoom (EscapeTechnion escapeTechnion, int faculty,
								 int id);

/* receives escapeTechnion, an escaper's email, faculty and skill level.
 * adds the escaper to the system.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_OUT_OF_MEMORY in case of memory allocation problems
 * returns ET_INVALID_PARAMETER if received an illegal parameter
 * returns ET_EMAIL_ALREADY_EXIST if the escaper already exists in the faculty
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult addEscaper (EscapeTechnion escapeTechnion, char* email,
								 int faculty, int skill_level);

/* receives escapeTechnion, an escaper's email and removes the escaper from
 * the system, freeing all of its allocated memory.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_EMAIL_DOES_NOT_EXIST if the escaper email isn't in the system
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult removeEscaper (EscapeTechnion escapeTechnion, char* email);

/* receives escapeTechnion, an escaper's email and order details and adds
 * the order for the given escaper.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_OUT_OF_MEMORY in case of memory allocation problems
 * returns ET_INVALID_PARAMETER if received an illegal parameter
 * returns ET_ID_DOES_NOT_EXIST if the escaper doesn't exist in the system
 * returns ET_SUCCESS otherwise
 *
 */
EscapeTechnionResult addOrder (EscapeTechnion escapeTechnion, char* email,
							   int faculty, int id, int day, int hour,
							   int num_ppl);

/* receives escapeTechnion, an escaper's email and the number of people an order
 * is for. finds the best room for the escaper and the number of people and
 * makes the earliest order possible.
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_OUT_OF_MEMORY in case of memory allocation problems
 * returns ET_INVALID_PARAMETER if received an illegal parameter
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult escaperRecommended (EscapeTechnion escapeTechnion,
										 char *email, int num_ppl);

/* receives escapeTechnion and an output file pointer. advances the system in
 * on day and prints all of the day's orders to the output file
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_OUT_OF_MEMOERY in case of memory allocation problems
 * returns ET_SUCCESS otherwise
 */
EscapeTechnionResult reportDay (EscapeTechnion escapeTechnion, FILE* output);

/* receives an escapeTechnion and an output file pointer. prints out
 * the NUM_BEST_FACULTIES (3) best earning faculties so far. if there is more
 * than 1 faculty with the same revenue prints in the order of their id's
 * returns ET_NULL_PARAMETER if one of the parameters is NULL
 * returns ET_SUCCESS otherwise  */
EscapeTechnionResult reportBestFaculties (EscapeTechnion escapeTechnion,
										  FILE* output);



#endif /* ESCAPETECHNION_H_ */
