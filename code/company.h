
#ifndef COMPANY_H_
#define COMPANY_H_
#include "assert.h"
#include <stdlib.h>
#include "room.h"
#include "company_input_checks.h"
typedef struct company_s* Company;

typedef enum{
	COMP_OUT_OF_MEMORY,
	COMP_NULL_PARAMETER,
	COMP_INVALID_PARAMETER,
	COMP_ID_ALREADY_EXISTS,
	COMP_ID_DOES_NOT_EXIST,
	COMP_NO_ROOMS_AVAILABLE,
	COMP_SUCCESS
}CompanyResult;

/* allocates a new company. receives it's email and faculty, sets them
 * and returns a pointer to the new company.
 * returns NULL if one of the parameters is NULL or illegal (if
 * email doesn't contain exactly one '@' character or if faculty isn't
 * defined), or if the memory allocation failed.
 * result is updated accordingly */
Company companyCreate(char* email, int faculty, CompanyResult *result);


/* frees all of company's allocated memory (including email). if company is
 * NULL does nothing. */
void companyDestroy(Company company);


/* compares between the two given companies' emails. assumes the given
 * companies, and their emails, aren't NULL.
 * returns a positive number if company1's email is lexicographically greater
 * than company2's email.
 * returns 0 is company1 and company2 have the same email
 * returns a negative number if company1's email is lexicographically smaller
 * than company2's email. */
int companyCompare(Company company1, Company company2);

/* returns a copy of the given company. returns NULL if the given company is
 * NULL or if there were memory problems. updates result accordingly */
Company companyCopy(Company company, CompanyResult *result);


/* receives a room's detail and it to the company.
 * returns COMP_OUT_OF_MEMORY in case of failure in memory allocation
 * return COMP_INVALID_PARAMETER if one of the room's details is illegal
 * returns COMP_ID_ALREADY_EXISTS if there's already a room in the company
 * with the same id
 * returns COMP_SUCCESS otherwise
 * assumes company isn't NULL
 */
CompanyResult addRoomToCompany(Company company, int room_id, int price,
							   int rec_num_ppl, int open_hour, int close_hour,
							   int difficulty);

/* receives a room's id, removes the room from the company and deallocates it's
 *  memory
 * returns COMP_ID_DOES_NOT_EXIST if the room isn't in the company
 * returns COMP_SUCCESS otherwise
 */
CompanyResult removeRoomFromCompany(Company company, int room_id);

/* receives a company. points the given pointer to a copy of its email.
 * returns COMP_SUCCESS unless there was a memory allocation problem, in which
 * case returns COMP_OUT_OF_MEMORY
 * returns COMP_NULL_PARAMETER if one of the given parameters is NULL
 */
CompanyResult getCompanyEmail(Company company, char** email);

/* receives a room's id and checks if the room is in the company
 * returns COMP_NULL_PARAMETER if one of the given parameters is NULL
 * returns COMP_SUCCESS otherwise and updates result if to true the room is
 * in the company, and to false if it isn't */
CompanyResult isRoomInCompany(Company company, int id, bool *result);

/* receives a company, number of people making an order, escaper's skill level
 * and updates the recommended room's id and faculty as well as the
 * recommendation score.
 * returns COMP_NULL_PARAMETER if one of the parameters is NULL
 * returns COMP_INVALID_PARAMETER is the skill level isn't between 1-10
 * or if the number of people is less than 1
 * returns COMP_NO_ROOMS_AVAILABLE if there are no rooms in the company
 * returns COMP_SUCCESS otherwise
 */
CompanyResult getRecommendedRoom(Company company, int num_ppl, int skill_level,
								 int *rec_id, int *rec_score);

/* receives a company and updates the given faculty parameter to the company's
 * faculty and return COMP_SUCCESS.
 * returns COMP_NULL_PARAMETER if one of the parameters is NULL
 */
CompanyResult getCompanyFaculty(Company company, int *faculty);

/*
 * receives a company and a room_id and assigns the room's difficulty to
 * the given parameter
 * returns COMPANY_ID_DOES_NOT_EXIST if the room doesn't exist
 * returns COMPANY_SUCCESS otherwise  */
CompanyResult getCompanyRoomDifficulty(Company company, int room_id,
								       int* difficulty);

/* receives a company and a room id, returns the price of the room.
 * returns COMPANY_NULL_PARAMETER if a given parameter is NULL
 * returns COMPANY_ID_DOES_NOT_EXIST if the room doesn't exist
 * returns COMPANY_SUCCESS otherwise
 */
CompanyResult getCompanyRoomPrice(Company company,int room_id, int *price);

/* receives a company and a room id, return the opening hours of the room.
 * returns COMPANY_NULL_PARAMETER if a given parameter is NULL
 * returns COMPANY_ID_DOES_NOT_EXIST if the room doesn't exist
 * returns COMPANY_SUCCESS otherwise
 */
CompanyResult getCompanyRoomOpenHours(Company company, int room_id,
									  int* open_hour, int *close_hour);

/* result is set true if the given company has rooms in it and to false
 * if there are no rooms in the company
 * returns COMP_NULL_PARAMETER if one of the parameters is NULL
 * returns COMP_SUCCESS otherwise
 */
CompanyResult companyAreThereRooms(Company company, bool* result);

#endif /* COMPANY_H_ */
