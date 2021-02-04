
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "EscapeTechnion.h"
#include "set.h"
#include "list.h"
#include "company.h"
#include "escaper.h"
#include "room.h"
#include "mtm_ex3.h"
#include "escape_technion_input_checks.h"

#define MIN_HOUR 0
#define MAX_HOUR 24
#define NUM_BEST_FACULTIES 3

struct s_escapeTechnion{
	Set companies, escapers;
	int* facultyProfits;
	int day;
};

/* receives escapeTechnion, a faculty, a room id and an hour. checks if the
 * given room is open in the given hour.
 * returns ET_NULL_PARAMETER if the given escapeTechnion is NULL
 * returns ET_ROOM_NOT_AVAILABLE if the room is closed in the given hour
 * returns ET_SUCCESS if the room is open in the given hour
 */
static EscapeTechnionResult isRoomOpen (EscapeTechnion escapeTechnion,
		int faculty, int id, int hour);

/* receives escapeTechnion, a faculty, a room id, a day and a hour.
 * checks if there are no orders made for the room in the given time
 * if the room doesn't have any orders made returns ET_SUCCESS
 * if the room has orders made returns ET_ROOM_NOT_AVAILABLE
 * if the given escapeTechnion is NULL returns ET_NULL_PARAMETER
 */
static EscapeTechnionResult isRoomAvailable (EscapeTechnion escapeTechnion,
		int faculty, int id, int day, int hour);

/* receives an escapeTechnion, a room id and a faculty. returns true  if the
 * room id belongs to one of the rooms in the faculty and false otherwise
 */
static bool isRoomIdTaken(EscapeTechnion escapeTechnion, int room_id,
					      int faculty, EscapeTechnionResult *result);

/* receives a company's email and returns the company from escapeTechnion's
 * set of companies.
 * returns NULL if the company wasn't found
 * assumes escapeTechnion isn't NULL */
static Company getCompanyByEmail(EscapeTechnion escapeTechnion, char* email,
								 EscapeTechnionResult *result);

/* a generic copy function that will be used for creating the companies set.
 * copies the given company and returns the copy. returns NULL if failed or if
 * the given company is NULL */
static void* genericCompanyCopy(void* company);

/* a generic destroy company function that will be used for creating the
 * companies set. deallocates all of the given company's memory. */
static void genericCompanyDestroy(void* company);

/* a generic compare companies function that will be used for creating the
 * companies set. compares between the companie's emails and returns-
 * a positive number: if comapny1's email is smaller (lexicographically)
 * than company2's
 * 0: if company1's email is the same as company2's
 * a negative number: if company1's email is greater(lexicographically)
 * than company2's
 * assumes the given companies and their emails are not NULL
 */
static int genericCompanyCompare(SetElement company1, SetElement company2);

/* a generic copy escaper function that will be used for creating the
 * escapers set. copies the given escaper and returns the copy. returns NULL
 * if failed or if the given escaper is NULL*/
static SetElement escaperCopyForSet(SetElement escaper);

/* a generic destroy escaper function that will be used for creating the
 * escapers set. deallocated the given escaper memory */
static void escaperDestroyForSet(SetElement escaper);

/* a generic compare escaper function that will be used for creating the
 * escapers set. compares between the two escapers' emails and return -
 * a positive number: if escaper1's email is smaller (lexicographically)
 * than escaper2's
 * 0: if escaper1's email is the same as escaper2's
 * a negative number: if escaper1's email is greater (lexicographically) than
 * escaper 2's
 */
static int escaperCompareForSet(SetElement escaper1, SetElement escaper2);

/* receives a CompanyResult type and returns the equivalent EscapeTechnionResult
 * value*/
static EscapeTechnionResult translateCompanyET(CompanyResult comp_res);

/* receives a SetResult type and returns the equivalent EscapeTechnionResult
 * value for its Company set */
static EscapeTechnionResult translateCompanySetET(SetResult set_res);

/* receives a SetResult type and returns equivalent EscapeTechnionResult value*/
static EscapeTechnionResult translateEscaperSetET(SetResult set_result);

/* receives an EscaperResult type and returns the equivalent EscapeTechnionRsult
 * value*/
static EscapeTechnionResult translateEscaperET(EscaperResult escaper_result);

/* receives a faculty and an id, returns a company from escapeTechnion's
 * set of companies that has the same faculty and a room with the same id.
 * returns NULL and updates result to ET_ID_DOES_NOT_EXIST if there is no
 * matching company
 * returns the company and updates result to ET_SUCCESS otherwise
 */
static Company getCompanyByFacultyAndRoomId(EscapeTechnion escapeTechnion,
											int faculty, int id,
											EscapeTechnionResult *result);

/* receives an escaper's email and an EscapeTechnion.
 * if the escaper is in escapeTechnion return it and updates result to
 * ET_SUCCESS. if the escaper is not found returns NULL and updates result to
 * ET_CLIENT_EMAIL_DOES_NOT_EXIST
 */
static Escaper getEscaperByEmail(EscapeTechnion escapeTechnion, char* email,
								 EscapeTechnionResult *result);

/* receives an EscapeTechnion, an escaper, number of people an order is being
 * made for, and pointers to the best id and best faculty
 * calculates the best score for the escaper and the number of people
 * updates the best id and the best faculty accordingly
 * returns ET_SUCCESS unless there are no rooms available, in which case returns
 * ET_NO_AVAILABLE_ROOMS*/
static EscapeTechnionResult calcBestScore(EscapeTechnion escapeTechnion,
										   Escaper escaper, int num_ppl,
										   int *best_id, int *best_faculty);

/* checks if the current score is better than the  recommended score.
 * if so updates the recommended score, faculty and id
 */
static void updateBestScore(int current_score, int* rec_score,int current_id,
							int* rec_id, int current_faculty,int* rec_faculty,
							int escaper_faculty);

/* returns the absolute value of num */
static int getAbsolute(int num);

/* receives an escapeTechnion, room id, a faculty, an escaper's email and the
 * number of people an order is for. makes the earliest order possible.
 * returns ET_OUT_OF_MEMORY if there were memory problems
 * returns ET_SUCCESS otherwise
 */
static EscapeTechnionResult addEarliestOrder(EscapeTechnion escapeTechnion,
											 int id, int faculty, char* email,
											 int num_ppl);

/* receives escapeTechnion and a parameter to put the result in.
 * makes a list of all the orders in escapeTechnion currently.
 * res is updated to ET_NULL_PARAMETER if one of the parameters is NULL
 * to ET_OUT_OF_MEMORY  if there were memory problems when creating the list
 * to ET_SUCCESS otherwise.
 * returns NULL if there are no orders
 */
static List getAllOrdersList(EscapeTechnion escapeTechnion,
							 EscapeTechnionResult *res);

/* a generic order copy function to be used when creating a list of orders */
static ListElement orderCopyForList(ListElement order);

/* a generic order destroy function to be used when creating a list of orders*/
static void orderDestroyForList(ListElement order);

/* check if the given room (corresponding to the faculty and the id) has any
 * orders */
static bool checkRoomOrders(EscapeTechnion escapeTechnion,int faculty,int id,
				            EscapeTechnionResult *res);

/* check if the given company has any orders */
static bool checkCompanyOrders(EscapeTechnion escapeTechnion, Company company,
							   EscapeTechnionResult *res);

/* returns a negative number if order1 is smaller than order2
 * returns zero if order1 is equal to order2
 * returns a positive number if order1 is greater than order2
 * the greater order is decided by comparing the hour. if the hours are equal
 * then the faculties are compared. if the faculties are equal, the room id's
 * are compared.
 */
static int compareOrdersForReport(ListElement order1, ListElement order2);

/* receives a ListResult type and returns the equivalent EscapeTechnionResult
 * value
 */
static EscapeTechnionResult translateListET(ListResult list_res);

/* receives an order. returns true if the order is for the day received
 *  and false otherwise
 */
static bool orderIsForToday(ListElement order, ListFilterKey day);

/* receives a list of orders and prints them by the order of the hour, faculty,
 * and room_id. updates the system's faculties profits */
static EscapeTechnionResult printOrdersList(EscapeTechnion escapeTechnion,
		List day_orders, FILE* output);

static Escaper getEscaperByOrder(EscapeTechnion escapeTechnion,
								 Order order, EscapeTechnionResult *result);
/* calculates the total price ( price * num of people ). calculates 75% if
 * the client faculty is the same as the room faculty
 */
static int calcPrice(int room_faculty, int client_faculty, int price,
					 int num_ppl);

/* receives escapeTechnion and removes today's orders from the system
 */
static void removeDayOrders(EscapeTechnion escapeTechnion);

/* receives escapeTechnion and returns it's total revenue */
static int getTotalRevenue(EscapeTechnion escapeTechnion);

/* receives an order, finds the escaper to which the order belongs and updates
 * the details
 */
static EscapeTechnionResult getEscaperDetails(Order order,
		 EscapeTechnion escapeTechnion, int* skill, char** email,
		 int* client_faculty);

/*  receives an order, finds the company to which the order belongs and updates
 * the details
 */
static EscapeTechnionResult getCompanyAndRoomDetails(Order order,
		EscapeTechnion escapeTechnion, char **company_email, int* difficulty,
		int *price, int room_id, int faculty);

/* receives an OrderResult type and returns the equivalent EscapeTechnionResult
 * value
 */
static EscapeTechnionResult translateOrderET(OrderResult order_res);

/* receives escapeTechnion and an email and checks if the email exists
 * in the system.  updates taken to true if it does and false if it doesnt.
 * assumes escapeTechnion and email are not NULL.
 * returns ET_OUT_OF_MEMORY in case of memory allocation problems
 * return ET_SUCCESS otherwise
 */
static EscapeTechnionResult isEmailTaken(EscapeTechnion escapeTechnion,
									   char* email, bool* taken);

/* receives escapeTechnion and a result pointer. result is updated to true
 * if there are any rooms in the escapeTechnion system and to false if there
 * are no rooms
 */
static EscapeTechnionResult areThereRooms(EscapeTechnion escapeTechnion,
										  bool* result);

/*
 * This function converts ListResult type messages to EscapeTechnion messages.
 */
static EscapeTechnionResult translateListET(ListResult list_res){
	switch (list_res) {
		case LIST_NULL_ARGUMENT:
			return ET_NULL_PARAMETER;
		case LIST_OUT_OF_MEMORY:
			return ET_OUT_OF_MEMORY;
		case LIST_SUCCESS:
			return ET_SUCCESS;
		default:
			return ET_SUCCESS;
	}
	return ET_SUCCESS;
}

/*
 * This function converts OrderResult type messages to EscapeTechnion messages.
 */
static EscapeTechnionResult translateOrderET(OrderResult order_res){
	switch (order_res){
		case ORDER_OUT_OF_MEMORY:
			return ET_OUT_OF_MEMORY;
		case ORDER_NULL_PARAMETER:
			return ET_NULL_PARAMETER;
		case ORDER_INVALID_PARAMETER:
			return ET_INVALID_PARAMETER;
		case ORDER_SUCCESS:
			return ET_SUCCESS;
		default: return ET_SUCCESS;
	}
}

/*
 * This function converts EscaperResult type messages to EscapeTechnion
 * messages.
 */
static EscapeTechnionResult translateEscaperET(EscaperResult escaper_result) {
	switch (escaper_result) {
		case ESCAPER_OUT_OF_MEMORY:
			return ET_OUT_OF_MEMORY;
		case ESCAPER_NULL_PARAMETER:
			return ET_NULL_PARAMETER;
		case ESCAPER_INVALID_PARAMETER:
			return ET_INVALID_PARAMETER;
		case ESCAPER_CLIENT_IN_ROOM:
			return ET_CLIENT_IN_ROOM;
		case ESCAPER_SUCCESS:
			return ET_SUCCESS;
		default: return -1;
	}
}

/*
 * This function converts SetResult of escaper set type messages to
 * EscapeTechnion messages.
 */
static EscapeTechnionResult translateEscaperSetET(SetResult set_result) {
	switch (set_result) {
		case SET_SUCCESS:
			return ET_SUCCESS;
		case SET_OUT_OF_MEMORY:
			return ET_OUT_OF_MEMORY;
		case SET_NULL_ARGUMENT:
			return ET_NULL_PARAMETER;
		case SET_ITEM_ALREADY_EXISTS:
			return ET_EMAIL_ALREADY_EXISTS;
		case SET_ITEM_DOES_NOT_EXIST:
			return ET_CLIENT_EMAIL_DOES_NOT_EXIST;
		default: return -1;
	}
}

/*
 * This function converts CompanyResult type messages to EscapeTechnion
 * messages.
 */
static EscapeTechnionResult translateCompanyET(CompanyResult comp_res){
	switch ( comp_res ) {
		case COMP_OUT_OF_MEMORY:
			return ET_OUT_OF_MEMORY;
		case COMP_NULL_PARAMETER:
			return ET_NULL_PARAMETER;
		case COMP_INVALID_PARAMETER:
			return ET_INVALID_PARAMETER;
		case COMP_SUCCESS:
			return ET_SUCCESS;
		case COMP_ID_ALREADY_EXISTS:
			return ET_ID_ALREADY_EXIST;
		case COMP_ID_DOES_NOT_EXIST:
			return ET_ID_DOES_NOT_EXIST;
		case COMP_NO_ROOMS_AVAILABLE:
			return ET_NO_ROOMS_AVAILABLE;
	}
	return ET_SUCCESS;
}

/*
 * This function converts SetResult type of company set messages to
 * EscapeTechnion messages.
 */
static EscapeTechnionResult translateCompanySetET(SetResult set_res){
	switch ( set_res ) {
		case SET_NULL_ARGUMENT:
			return ET_NULL_PARAMETER;
		case SET_OUT_OF_MEMORY:
			return ET_OUT_OF_MEMORY;
		case SET_ITEM_ALREADY_EXISTS:
			return ET_EMAIL_ALREADY_EXISTS;
		case SET_ITEM_DOES_NOT_EXIST:
			return ET_COMPANY_EMAIL_DOES_NOT_EXIST;
		case SET_SUCCESS:
			return ET_SUCCESS;
	}
	return ET_SUCCESS;
}


EscapeTechnion addEscapeTechnion (EscapeTechnionResult *result){
	assert(result!=NULL);
	EscapeTechnion escapeTechnion = malloc(sizeof(*escapeTechnion));
	Set companies = setCreate(genericCompanyCopy, genericCompanyDestroy,
							  genericCompanyCompare);
	Set escapers = setCreate(escaperCopyForSet, escaperDestroyForSet,
						     escaperCompareForSet);
	int num_of_faculties = UNKNOWN;
	int* profits = malloc(sizeof(*profits)*num_of_faculties);
	if ( escapeTechnion == NULL || companies == NULL || escapers == NULL
		 || profits == NULL ){
		*result = ET_OUT_OF_MEMORY;
		free(escapeTechnion);
		escapeTechnion = NULL;
		free(companies);
		companies = NULL;
		free(escapers);
		escapers = NULL;
		free(profits);
		profits = NULL;
		return NULL;
	}
	escapeTechnion->companies = companies;
	escapeTechnion->escapers = escapers;
	escapeTechnion->facultyProfits = profits;
	for (int i=0; i<num_of_faculties; i++){ //initialize profits to 0
		*((escapeTechnion->facultyProfits)+i) = 0;
	}
	escapeTechnion->day = 0;
	*result = ET_SUCCESS;
	return escapeTechnion;
}

EscapeTechnion copyEscapeTechnion (EscapeTechnion escapeTechnion,
								   EscapeTechnionResult *result){
	assert(result!=NULL);
	if ( escapeTechnion == NULL ){
		*result = ET_NULL_PARAMETER;
		return NULL;
	}
	EscapeTechnion new_et = malloc(sizeof(*new_et));
	Set new_companies = setCopy(escapeTechnion->companies);
	Set new_escapers = setCopy(escapeTechnion->escapers);
	if ( new_et == NULL || new_companies == NULL || new_escapers == NULL ){
		*result = ET_OUT_OF_MEMORY;
		return NULL;
	}
	int* profits_copy = malloc(sizeof(*profits_copy)*UNKNOWN);
	if ( profits_copy == NULL ){
		free(new_et);
		setDestroy(new_companies);
		setDestroy(new_escapers);
		*result = ET_OUT_OF_MEMORY;
		return NULL;
	}
	for ( int i=0; i<UNKNOWN; i++){
		profits_copy[i] = (escapeTechnion->facultyProfits)[i];
		i++;
	}
	new_et->companies = new_companies;
	new_et->escapers = new_escapers;
	new_et->facultyProfits = profits_copy;
	new_et->day = escapeTechnion->day;
	*result = ET_SUCCESS;
	return new_et;
}

void destroyEscapeTechnion (EscapeTechnion escapeTechnion){
	if (escapeTechnion == NULL){
		return;
	}
	setDestroy(escapeTechnion->companies);
	setDestroy(escapeTechnion->escapers);
	free(escapeTechnion->facultyProfits);
	escapeTechnion->facultyProfits = NULL;
	free(escapeTechnion);
	escapeTechnion = NULL;
	return;
}

EscapeTechnionResult addCompany (EscapeTechnion escapeTechnion, char* email,
								 int faculty){
	CompanyResult comp_res;
	if ( !isLegalEmail(email) || !isLegalFaculty(faculty) ){
		return ET_INVALID_PARAMETER;
	}
	bool taken;
	EscapeTechnionResult et_res = isEmailTaken(escapeTechnion, email, &taken);
	if ( et_res != ET_SUCCESS ){
		return et_res;
	}
	if ( taken ){
		return ET_EMAIL_ALREADY_EXISTS;
	}
	Company company = companyCreate(email, faculty, &comp_res);
	SetResult set_res;
	if ( comp_res == COMP_SUCCESS ){
		set_res = setAdd(escapeTechnion->companies, company);
		companyDestroy(company); // free the allocated memory
		return translateCompanySetET(set_res);
	}
	return translateCompanyET(comp_res);
}

EscapeTechnionResult removeCompany (EscapeTechnion escapeTechnion, char *email){
	if ( escapeTechnion== NULL || email == NULL ){
		return ET_NULL_PARAMETER;
	}
	if ( !isLegalEmail(email) ){
		return ET_INVALID_PARAMETER;
	}
	SetResult set_res;
	EscapeTechnionResult res;
	Company removed_company = getCompanyByEmail(escapeTechnion, email, &res);
	if ( res != ET_SUCCESS ){
		return res;
	}
	if ( checkCompanyOrders(escapeTechnion, removed_company, &res) ){
		return ET_RESERVATION_EXIST;
	}
	if ( res != ET_SUCCESS ){
		return res;
	}
	set_res = setRemove(escapeTechnion->companies, removed_company);
	res = translateCompanySetET(set_res);
	return res;
}

EscapeTechnionResult addRoom (EscapeTechnion escapeTechnion, char *email,
							  int id, int price, int rec_num_ppl,
							  int open_hour, int close_hour, int difficulty){
	if ( escapeTechnion == NULL || email == NULL ){
		return ET_NULL_PARAMETER;
	}
	if (!isLegalEmail(email) || !isLegalId(id) || !isLegalPrice(price)
		|| !isLegalNumPpl(rec_num_ppl) || !isLegalDifficulty(difficulty)
	    || !isLegalOpeningHours(open_hour, close_hour)){
		return ET_INVALID_PARAMETER;
	}
	EscapeTechnionResult et_res;
	Company company = getCompanyByEmail(escapeTechnion, email, &et_res);
	if (et_res != ET_SUCCESS){
		return et_res;
	}
	int faculty;
	getCompanyFaculty(company, &faculty);
	if ( isRoomIdTaken(escapeTechnion, id, faculty, &et_res) ){
		return ET_ID_ALREADY_EXIST;
	}
	if ( et_res != ET_SUCCESS ){
		return et_res;
	}
	CompanyResult comp_res = addRoomToCompany(company, id, price, rec_num_ppl,
											  open_hour, close_hour,
											  difficulty);
	return translateCompanyET(comp_res);
}

EscapeTechnionResult removeRoom (EscapeTechnion escapeTechnion, int faculty,
								 int id){
	if ( escapeTechnion == NULL ){
		return ET_NULL_PARAMETER;
	}
	EscapeTechnionResult et_res;
	CompanyResult comp_res;
	if ( !isLegalFaculty(faculty) || !isLegalId(id) ){
		return ET_INVALID_PARAMETER;
	}
	Company company_of_room = getCompanyByFacultyAndRoomId(escapeTechnion,
														   faculty,id,&et_res);
	if ( et_res == ET_SUCCESS ){
		if ( checkRoomOrders(escapeTechnion, faculty, id, &et_res) ){
			return ET_RESERVATION_EXIST;
		}
		if ( et_res != ET_SUCCESS ){
			return et_res;
		}
		comp_res = removeRoomFromCompany(company_of_room, id);
		return translateCompanyET(comp_res);
	}
	return et_res;
}

EscapeTechnionResult addEscaper (EscapeTechnion escapeTechnion,
								 char* email, int faculty, int skill_level) {
	if (escapeTechnion == NULL || email == NULL) {
		return ET_NULL_PARAMETER;
	}
	if (!isLegalEmail(email) || !isLegalFaculty(faculty) ||
			!isLegalSkillLevel(skill_level)){
		return ET_INVALID_PARAMETER;
	}
	bool taken;
	EscapeTechnionResult ET_res = isEmailTaken(escapeTechnion, email, &taken);
	if ( ET_res != ET_SUCCESS ){
		return ET_res;
	}
	if ( taken ){
		return ET_EMAIL_ALREADY_EXISTS;
	}
	EscaperResult escaper_res;
	Escaper new_escaper = escaperCreate(email, faculty, skill_level,
			&escaper_res);
	ET_res = translateEscaperET(escaper_res);
	if (ET_res != ET_SUCCESS) {
		return ET_res;
	}
	if (setIsIn(escapeTechnion->escapers, new_escaper)) {
		escaperDestroy(new_escaper);
		return ET_EMAIL_ALREADY_EXISTS;
	}
	SetResult set_res = setAdd(escapeTechnion->escapers, new_escaper);
	ET_res = translateEscaperSetET(set_res);
	escaperDestroy(new_escaper);
	return ET_res;
}


EscapeTechnionResult removeEscaper (EscapeTechnion escapeTechnion,
									char* email) {
	if (escapeTechnion == NULL || email == NULL) {
		return ET_NULL_PARAMETER;
	}
	if (!isLegalEmail(email)) {
		return ET_INVALID_PARAMETER;
	}
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	while (escaper != NULL) {
		char* escaper_email;
		EscaperResult res = escaperGetEmail(escaper,&escaper_email);
		EscapeTechnionResult result = translateEscaperET(res);
		if (result != ET_SUCCESS) {
			free(escaper_email);
			return result;
		}
		if (strcmp(email,escaper_email)==0) {
			SetResult res = setRemove(escapeTechnion->escapers,escaper);
			EscapeTechnionResult result = translateEscaperSetET(res);
			free(escaper_email);
			return result;
		}
		free(escaper_email);
		escaper_email = NULL;
		escaper = setGetNext(escapeTechnion->escapers);
	}
	return ET_CLIENT_EMAIL_DOES_NOT_EXIST;
}

static EscapeTechnionResult isRoomOpen (EscapeTechnion escapeTechnion,
										int faculty, int id, int hour) {
	EscapeTechnionResult et_res;
	Company company = getCompanyByFacultyAndRoomId(escapeTechnion, faculty, id,
			  &et_res);
	if ( et_res != ET_SUCCESS){
		return et_res;
	}
	if ( company == NULL ){
		return et_res;
	}
	int room_open, room_close;
	CompanyResult comp_res = getCompanyRoomOpenHours(company, id, &room_open,
														 &room_close);
	et_res = translateCompanyET(comp_res);
	if ( et_res != ET_SUCCESS ){
		return et_res;
	}
	if (!(room_open<=hour && hour<room_close)){
		return ET_ROOM_NOT_AVAILABLE;
	}
	return ET_SUCCESS;
}


static EscapeTechnionResult isRoomAvailable (EscapeTechnion escapeTechnion,
											 int faculty, int id, int day,
											 int hour) {
	if (escapeTechnion == NULL) {
		return ET_NULL_PARAMETER;
	}
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	List orders;
	int order_faculty, order_id, order_hour, order_day;
	while (escaper!= NULL) {
		EscaperResult es_res = escaperListOrders(escaper, &orders);
		EscapeTechnionResult et_res = translateEscaperET(es_res);
		if (et_res != ET_SUCCESS) {
			return et_res;
		}
		Order order = listGetFirst(orders);
		while (order != NULL) {
			OrderResult order_res = orderGetDay(order,&order_day);
			et_res = translateEscaperET(translateOrderEscaper(order_res));
			if ( et_res!= ET_SUCCESS) {
				listDestroy(orders);
				return et_res;
			}
			order_res = orderGetHour(order,&order_hour);
			et_res = translateEscaperET(translateOrderEscaper(order_res));
			if ( et_res!= ET_SUCCESS) {
				listDestroy(orders);
				return et_res;
			}
			order_res = orderGetRoomId(order,&order_id);
			et_res = translateEscaperET(translateOrderEscaper(order_res));
			if ( et_res!= ET_SUCCESS) {
				listDestroy(orders);
				return et_res;
			}
			order_res = orderGetRoomFaculty(order,&order_faculty);
			et_res = translateOrderET(translateOrderEscaper(order_res));
			if ( et_res!= ET_SUCCESS) {
				listDestroy(orders);
				return et_res;
			}
			if (day == order_day && hour == order_hour &&
				faculty == order_faculty && id == order_id) {
				listDestroy(orders);
				return ET_ROOM_NOT_AVAILABLE;
			}
			order = listGetNext(orders);
		}
		escaper = setGetNext(escapeTechnion->escapers);
		listDestroy(orders);
		orders = NULL;
	}
	return ET_SUCCESS;
}


EscapeTechnionResult addOrder (EscapeTechnion escapeTechnion, char* email,
							   int faculty, int id, int day, int hour,
							   int num_ppl) {
	if (escapeTechnion == NULL || email == NULL) {
		return ET_NULL_PARAMETER;
	}
	if (!isLegalEmail(email) || !isLegalFaculty(faculty) || !isLegalId(id) ||
		!isLegalDay(day) || !isLegalOrderHour(hour) || !isLegalNumPpl(num_ppl)){
		return ET_INVALID_PARAMETER;
	}
	EscapeTechnionResult et_result;
	EscapeTechnionResult et_res;
	Escaper escaper = getEscaperByEmail(escapeTechnion, email, &et_res);
	if ( escaper != NULL ){
		bool inRoom;
		if (!isRoomIdTaken(escapeTechnion, id, faculty, &et_result)) {
			return ET_ID_DOES_NOT_EXIST;
		}
		EscaperResult es_res = isEscaperInRoom(escaper, hour, day, &inRoom);
		if (translateEscaperET(es_res)!=ET_SUCCESS){
			return translateEscaperET(es_res);
		}
		if (inRoom){
			return ET_CLIENT_IN_ROOM;
		}
		et_result = isRoomAvailable(escapeTechnion, faculty, id, day, hour);
		if (et_result != ET_SUCCESS) {
			return et_result;
		}
		et_result = isRoomOpen (escapeTechnion, faculty, id, hour);
		if (et_result != ET_SUCCESS) {
			return et_result;
		}
		es_res =  escaperAddOrder (escaper, faculty, id, day,hour, num_ppl);
		et_result = translateEscaperET(es_res);
		return et_result;
	}
	return ET_CLIENT_EMAIL_DOES_NOT_EXIST;
}

EscapeTechnionResult escaperRecommended (EscapeTechnion escapeTechnion,
										 char *email, int num_ppl){
	if ( email == NULL || escapeTechnion == NULL ){
		return ET_NULL_PARAMETER;
	}
	if ( !isLegalNumPpl(num_ppl) || !isLegalEmail(email) ){
		return ET_INVALID_PARAMETER;
	}
	EscapeTechnionResult et_res;
	Escaper escaper = getEscaperByEmail(escapeTechnion, email, &et_res);
	if ( escaper == NULL ){
		return et_res;
	}
	bool thereAreRooms;
	et_res = areThereRooms(escapeTechnion, &thereAreRooms);
	if ( et_res == ET_SUCCESS && !thereAreRooms ){
		return ET_NO_ROOMS_AVAILABLE;
	}
	int best_id, best_faculty;
	et_res = calcBestScore(escapeTechnion, escaper, num_ppl, &best_id,
					       &best_faculty);
	if ( et_res != ET_SUCCESS ){
		return et_res;
	}
	et_res = addEarliestOrder(escapeTechnion, best_id, best_faculty, email,
							  num_ppl);
	return et_res;

}

static EscapeTechnionResult addEarliestOrder(EscapeTechnion escapeTechnion,
											 int id, int faculty,
											 char* email, int num_ppl){
	assert(escapeTechnion!=NULL && email!=NULL);
	int day=0, hour=0;
	EscapeTechnionResult et_res;
	et_res = addOrder(escapeTechnion, email, faculty, id, day, hour, num_ppl);
	while ( et_res != ET_SUCCESS ){
		if ( et_res == ET_OUT_OF_MEMORY ){
			return et_res;
		}
		hour++;
		if ( hour > MAX_HOUR-1 ){
			hour=0;
			day++;
		}
		et_res=addOrder(escapeTechnion, email, faculty, id, day, hour, num_ppl);
	}
	return et_res;
}

static EscapeTechnionResult calcBestScore(EscapeTechnion escapeTechnion,
										  Escaper escaper, int num_ppl,
										  int *best_id, int *best_faculty){
	EscapeTechnionResult et_res;
	assert(escaper!=NULL && escapeTechnion!=NULL && best_id!=NULL &&
		   best_faculty!=NULL);
	int skill_level, escaper_faculty;
	escaperGetFaculty(escaper, &escaper_faculty);
	escaperGetSkillLevel(escaper, &skill_level);
	int current_faculty, rec_faculty, current_score, rec_score;
	bool got_first_rec = false;
	Company current_company = setGetFirst(escapeTechnion->companies);
	if ( current_company == NULL ){
		return ET_NO_ROOMS_AVAILABLE;
	}
	int current_id, rec_id;
	CompanyResult comp_res;
	et_res = ET_SUCCESS;
	while ( current_company != NULL ){
		getCompanyFaculty(current_company, &current_faculty);
		if ( !got_first_rec ){
			comp_res=getRecommendedRoom(current_company,num_ppl, skill_level,
										&rec_id, &rec_score);
			if ( comp_res == COMP_SUCCESS ){
				got_first_rec = true;
				getCompanyFaculty(current_company, &rec_faculty);
				et_res = ET_SUCCESS;
			}
			current_company = setGetNext(escapeTechnion->companies);
			continue;
		}
		comp_res = getRecommendedRoom(current_company, num_ppl, skill_level,
								      &current_id, &current_score);
		if ( comp_res != COMP_SUCCESS ){
			if ( comp_res != COMP_NO_ROOMS_AVAILABLE ){
				return translateCompanyET(comp_res);
			}
			current_company = setGetNext(escapeTechnion->companies);
			continue;
		}
		et_res = translateCompanyET(comp_res);
		updateBestScore(current_score, &rec_score, current_id, &rec_id,
						current_faculty, &rec_faculty, escaper_faculty);
		current_company = setGetNext(escapeTechnion->companies);
	}
	*best_id = rec_id;
	*best_faculty = rec_faculty;
	return et_res;
}

static void updateBestScore(int current_score, int* rec_score,int current_id,
							int* rec_id, int current_faculty,int* rec_faculty,
							int escaper_faculty){
	int diff_rec, diff_current;
	assert(rec_score!=NULL && rec_id!=NULL && rec_faculty!=NULL);
	if ( current_score <= *rec_score ){
		if ( current_score < *rec_score ){
			*rec_score = current_score;
			*rec_id = current_id;
			*rec_faculty = current_faculty;
			return;
		}
		diff_rec = getAbsolute(*rec_faculty - escaper_faculty);
		diff_current = getAbsolute(current_faculty - escaper_faculty);
		if ( diff_current <= diff_rec ){
			if ( diff_current < diff_rec )
			{
				*rec_score = current_score;
				*rec_id = current_id;
				*rec_faculty = current_faculty;
				return;
			}
			if ( current_faculty < *rec_faculty ){
				*rec_score = current_score;
				*rec_id = current_id;
				*rec_faculty = current_faculty;
				return;
			}
		}
	}
}

EscapeTechnionResult reportDay (EscapeTechnion escapeTechnion, FILE* output){
	if (escapeTechnion == NULL || output == NULL){
		return ET_NULL_PARAMETER;
	}
	EscapeTechnionResult res;
	List all_orders = getAllOrdersList(escapeTechnion, &res);
	if ( res != ET_SUCCESS ){
		return res;
	}
	ListResult sort_res = listSort(all_orders, compareOrdersForReport);
	res = translateListET(sort_res);
	if ( res!= ET_SUCCESS ){
			return res;
	}
	int zero = 0;
	List day_orders = listFilter(all_orders, orderIsForToday, &zero);
	listDestroy(all_orders);
	all_orders = NULL;
	if ( day_orders == NULL ){
		return ET_OUT_OF_MEMORY;
	}
	int num_of_events = listGetSize(day_orders);
	mtmPrintDayHeader(output, escapeTechnion->day, num_of_events);
	if (day_orders != NULL ){
		printOrdersList(escapeTechnion, day_orders, output);
	}
	mtmPrintDayFooter(output, escapeTechnion->day);
	removeDayOrders(escapeTechnion);
	escapeTechnion->day = (escapeTechnion->day)+1;
	listDestroy(day_orders);
	day_orders = NULL;
	return ET_SUCCESS;
}

EscapeTechnionResult reportBestFaculties (EscapeTechnion escapeTechnion,
										  FILE* output){
	if ( escapeTechnion == NULL || output == NULL ){
		return ET_NULL_PARAMETER;
	}
	int total_revenue = getTotalRevenue(escapeTechnion);
	int num_of_faculties = UNKNOWN;
	mtmPrintFacultiesHeader(output, num_of_faculties, escapeTechnion->day,
							total_revenue);
	int *profits = escapeTechnion->facultyProfits;
	int max1=-1, max2=-1, max3=-1, i1=0, i2=0, i3=0;
	for (int i=0; i<num_of_faculties; i++){
		if ( profits[i] > max3 ){
			if ( profits[i] > max2 ){
				if ( profits[i] > max1 ){
					max3 = max2;
					i3 = i2;
					max2 = max1;
					i2 = i1;
					max1 = profits[i];
					i1 = i;
					continue;
				}
				max3 = max2;
				i3 = i2;
				max2 = profits[i];
				i2 = i;
				continue;
			}
			max3 = profits[i];
			i3 = i;
		}
	}
	mtmPrintFaculty(output, (TechnionFaculty)i1, profits[i1]);
	mtmPrintFaculty(output, (TechnionFaculty)i2, profits[i2]);
	mtmPrintFaculty(output, (TechnionFaculty)i3, profits[i3]);
	mtmPrintFacultiesFooter(output);
	return ET_SUCCESS;
}

static int getTotalRevenue(EscapeTechnion escapeTechnion){
	int num_of_faculties = UNKNOWN;
	int revenue = 0;
	for (int i=0; i<num_of_faculties; i++){
		revenue+=(escapeTechnion->facultyProfits)[i];
	}
	return revenue;
}


static List getAllOrdersList(EscapeTechnion escapeTechnion,
							 EscapeTechnionResult *res){
	assert(res!=NULL);
	if (escapeTechnion == NULL){
		*res = ET_NULL_PARAMETER;
		return NULL;
	}
	List all_orders = listCreate(orderCopyForList, orderDestroyForList);
	if ( all_orders == NULL ){
		*res = ET_OUT_OF_MEMORY;
		return NULL;
	}
	Order order;
	List escaper_orders;
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	while ( escaper!=NULL ){
		EscaperResult es_res = escaperListOrders(escaper, &escaper_orders);
		if ( es_res != ESCAPER_SUCCESS ){
			listDestroy(all_orders);
			*res =  translateEscaperET(es_res);
			return NULL;
		}
		order = listGetFirst(escaper_orders);
		while ( order!= NULL ){
			listInsertFirst(all_orders, order);
			order = listGetNext(escaper_orders);
		}
		listDestroy(escaper_orders);
		escaper = setGetNext(escapeTechnion->escapers);
	}
	*res = ET_SUCCESS;
	return all_orders;
}

static void* genericCompanyCopy(void* company){
	CompanyResult res;
	Company copied_company = companyCopy((Company)company, &res);
	return copied_company;
}

static void genericCompanyDestroy(void* company){
	companyDestroy((Company)company);
	return;
}

static int genericCompanyCompare(void* company1, void* company2){
	return companyCompare((Company)company1, (Company)company2);
}

static SetElement escaperCopyForSet(SetElement escaper){
	EscaperResult res;
	Escaper copied_escaper = escaperCopy((Escaper)escaper, &res);
	return copied_escaper;
}

static void escaperDestroyForSet(SetElement escaper){
	escaperDestroy((Escaper)escaper);
	return;
}

static int escaperCompareForSet(SetElement escaper1, SetElement escaper2){
	return escaperCompare((Escaper)escaper1, (Escaper)escaper2);
}

static Company getCompanyByEmail(EscapeTechnion escapeTechnion, char* email,
								 EscapeTechnionResult *result){
	assert(escapeTechnion!=NULL && email!=NULL && result!=NULL );
	Company current = setGetFirst(escapeTechnion->companies);
	char* current_email = NULL;
	CompanyResult comp_res;
	while (current != NULL ){
		comp_res = getCompanyEmail(current, &current_email);
		if ( comp_res != COMP_SUCCESS ){
			free(current_email);
			current_email = NULL;
			*result = translateCompanyET(comp_res);
			return NULL;
		}
		if ( strcmp(current_email, email) == 0 ){
			free(current_email);
			current_email = NULL;
			*result = ET_SUCCESS;
			return current;
		}
		current = setGetNext(escapeTechnion->companies);
		free(current_email);
		current_email = NULL;
	}
	*result = ET_COMPANY_EMAIL_DOES_NOT_EXIST;
	return NULL;
}

static Company getCompanyByFacultyAndRoomId(EscapeTechnion escapeTechnion,
											int faculty, int id,
											EscapeTechnionResult *result){
	assert(result!=NULL && escapeTechnion!=NULL);
	Company current = setGetFirst(escapeTechnion->companies);
	bool in_room;
	int current_faculty;
	while ( current!=NULL) {
		getCompanyFaculty(current, &current_faculty);
		while ( current!=NULL && current_faculty!=faculty ){
			current = setGetNext(escapeTechnion->companies);
			getCompanyFaculty(current, &current_faculty);
		}
		if ( current == NULL ){
			break;
		}
		isRoomInCompany(current, id, &in_room);
		if ( in_room ){
			*result = ET_SUCCESS;
			return current;
		}
		current = setGetNext(escapeTechnion->companies);
	}
	*result = ET_ID_DOES_NOT_EXIST;
	return NULL;

}


static Escaper getEscaperByEmail(EscapeTechnion escapeTechnion, char* email,
								 EscapeTechnionResult *result){
	assert(escapeTechnion!=NULL && email!=NULL && result!=NULL);
	Escaper current_escaper = setGetFirst(escapeTechnion->escapers);
	char* current_email;
	EscaperResult es_res;
	while ( current_escaper!=NULL ){
		es_res = escaperGetEmail( current_escaper, &current_email );
		if ( translateEscaperET(es_res) != ET_SUCCESS ){
			free(current_email);
			current_email = NULL;
			*result = translateEscaperET(es_res);
			return NULL;
		}
		if (strcmp(current_email, email) == 0){
			free(current_email);
			current_email = NULL;
			*result = ET_SUCCESS;
			return current_escaper;
		}
		free(current_email);
		current_email = NULL;
		current_escaper = setGetNext(escapeTechnion->escapers);
	}
	*result = ET_CLIENT_EMAIL_DOES_NOT_EXIST;
	return NULL;
}

static int getAbsolute(int num){
	return num < 0 ? -num : num;
}

static bool isRoomIdTaken(EscapeTechnion escapeTechnion, int room_id,
					      int faculty, EscapeTechnionResult *result){
	assert(escapeTechnion!=NULL && result!=NULL);
	*result = ET_SUCCESS;
	Company company = setGetFirst(escapeTechnion->companies);
	CompanyResult comp_res;
	int company_faculty;
	while ( company!= NULL ){
		comp_res = getCompanyFaculty(company, &company_faculty);
		if ( comp_res != COMP_SUCCESS ){
			*result = translateCompanyET(comp_res);
			return false;
		}
		if ( company_faculty == faculty ){
			bool in_company ;
			comp_res = isRoomInCompany(company, room_id, &in_company);
			if ( comp_res != COMP_SUCCESS ){
				*result = translateCompanyET(comp_res);
				return false;
			}
			if ( in_company ){
				return true;
			}
		}
		company = setGetNext(escapeTechnion->companies);
	}
	return false;
}

static bool checkRoomOrders(EscapeTechnion escapeTechnion,int faculty,int id,
							EscapeTechnionResult *res){
	assert(escapeTechnion!=NULL && res!=NULL);
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	EscaperResult es_res;
	bool found = false;
	*res = ET_SUCCESS;
	while ( escaper!=NULL ){
		es_res = searchMatchingOrder(escaper, faculty, id, &found);
		if ( es_res != ESCAPER_SUCCESS ){
			*res = translateEscaperET(es_res);
			return false;
		}
		if ( found ){
			return true;
		}
		escaper = setGetNext(escapeTechnion->escapers);
	}
	return false;
}

static bool checkCompanyOrders(EscapeTechnion escapeTechnion, Company company,
							   EscapeTechnionResult *res){
	assert(escapeTechnion!=NULL && company!=NULL && res!=NULL );
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	EscaperResult es_res;
	EscapeTechnionResult et_res;
	List escaper_orders;
	Company order_company;
	int order_faculty, order_id;
	while ( escaper!=NULL ){
		es_res = escaperListOrders(escaper, &escaper_orders);
		if ( es_res != ESCAPER_SUCCESS ){
			*res = translateEscaperET(es_res);
			listDestroy(escaper_orders);
			return false;
		}
		Order order = listGetFirst(escaper_orders);
		while ( order!=NULL ){
			orderGetRoomFaculty(order, &order_faculty);
			orderGetRoomId(order, &order_id);
			order_company = getCompanyByFacultyAndRoomId(escapeTechnion,
							order_faculty, order_id, &et_res);
			if ( companyCompare(order_company, company) == 0 ){
				listDestroy(escaper_orders);
				return true;
			}
			order = listGetNext(escaper_orders);
		}
		listDestroy(escaper_orders);
		escaper = setGetNext(escapeTechnion->escapers);
	}
	return false;
}

static ListElement orderCopyForList(ListElement order){
	OrderResult res;
	Order new_order = orderCopy((Order)order, &res);
	return (ListElement)new_order;
}

static void orderDestroyForList(ListElement order){
	orderDestroy((Order)order);
	return;
}

static int compareOrdersForReport(ListElement order1, ListElement order2){
	int hour1, hour2, faculty1, faculty2, room_id1, room_id2;
	OrderResult res = orderGetHour((Order)order1, &hour1);
	assert(res==ORDER_SUCCESS);
	res = orderGetHour((Order)order2, &hour2);
	assert(res==ORDER_SUCCESS);
	if ( hour1 != hour2 ){
		return hour1-hour2;
	}
	res = orderGetRoomFaculty((Order)order1, &faculty1);
	assert(res==ORDER_SUCCESS);
	res = orderGetRoomFaculty((Order)order2, &faculty2);
	assert(res==ORDER_SUCCESS);
	if ( faculty1 != faculty2 ){
		return faculty1-faculty2;
	}
	res = orderGetRoomId((Order)order1, &room_id1);
	assert(res==ORDER_SUCCESS);
	res = orderGetRoomId((Order)order2, &room_id2);
	assert(res==ORDER_SUCCESS);
	return room_id1-room_id2;
}

static bool orderIsForToday(ListElement order, ListFilterKey day){
	if (order == NULL || day == NULL){
		return false;
	}
	int order_day;
	orderGetDay((Order)order, &order_day);
	return ( order_day == *((int*)day) );
}

static EscapeTechnionResult printOrdersList(EscapeTechnion escapeTechnion,
											List day_orders, FILE* output){
	char *email, *company_email;
	int skill, id, hour, difficulty, num_ppl, totalPrice, client_faculty,
	room_faculty, price;
	Order order = listGetFirst(day_orders);
	EscapeTechnionResult et_res;
	while ( order != NULL ){ // go through all of today's orders
		OrderResult o_res = orderGetRoomFaculty(order, &room_faculty);
		if ( o_res != ORDER_SUCCESS ){
			return ET_INVALID_PARAMETER;
		}
		o_res = orderGetRoomId(order, &id);
		if ( o_res != ORDER_SUCCESS ){
			return ET_INVALID_PARAMETER;
		}
		o_res = orderGetHour(order, &hour);
		if ( o_res != ORDER_SUCCESS ){
			return ET_INVALID_PARAMETER;
		}
		o_res = orderGetNumPpl(order, &num_ppl);
		if ( o_res != ORDER_SUCCESS ){
			return ET_INVALID_PARAMETER;
		}
		et_res = getEscaperDetails(order, escapeTechnion, &skill, &email,
								   &client_faculty);
		if ( et_res != ET_SUCCESS ){
			return et_res;
		}

		et_res = getCompanyAndRoomDetails(order, escapeTechnion, &company_email,
				&difficulty, &price, id, room_faculty);
		if ( et_res != ET_SUCCESS ){
					return et_res;
		}
		totalPrice = calcPrice(room_faculty, client_faculty, price, num_ppl);
		mtmPrintOrder(output, email, skill, (TechnionFaculty) client_faculty,
				company_email, (TechnionFaculty) room_faculty, id, hour,
				difficulty, num_ppl, totalPrice);
		(escapeTechnion->facultyProfits)[room_faculty]+=totalPrice;
		order = listGetNext(day_orders);
		free(email);
		email = NULL;
		free(company_email);
		company_email = NULL;
	}
	return ET_SUCCESS;
}

static EscapeTechnionResult getEscaperDetails(Order order,
											  EscapeTechnion escapeTechnion,
											  int* skill, char** email,
											  int* client_faculty){
	EscapeTechnionResult et_res;
	Escaper escaper = getEscaperByOrder(escapeTechnion, order, &et_res);
	if ( et_res != ET_SUCCESS ){
		return et_res;
	}
	EscaperResult es_res = escaperGetEmail(escaper, email);
	if ( es_res != ESCAPER_SUCCESS ){
		return translateEscaperET(es_res);
	}
	es_res = escaperGetSkillLevel(escaper, skill);
	if ( es_res != ESCAPER_SUCCESS ){
		free(email);
		return translateEscaperET(es_res);
	}
	es_res = escaperGetFaculty(escaper, client_faculty);
	if ( es_res != ESCAPER_SUCCESS ){
		free(email);
		return translateEscaperET(es_res);
	}
	return ET_SUCCESS;
}

static EscapeTechnionResult getCompanyAndRoomDetails(Order order,
		EscapeTechnion escapeTechnion, char **company_email, int* difficulty,
		int *price, int room_id, int faculty){
	EscapeTechnionResult et_res;
	Company company = getCompanyByFacultyAndRoomId(escapeTechnion, faculty,
												   room_id, &et_res);
	if ( et_res != ET_SUCCESS ){
		return et_res;
	}
	CompanyResult comp_res = getCompanyEmail(company, company_email);
	if ( comp_res != COMP_SUCCESS ){
		return translateCompanyET(comp_res);
	}
	comp_res=getCompanyRoomDifficulty(company, room_id, difficulty);
	if ( comp_res != COMP_SUCCESS ){
		free(company_email);
		return translateCompanyET(comp_res);
	}
	comp_res = getCompanyRoomPrice(company, room_id, price);
	if ( comp_res != COMP_SUCCESS ){
		free(company_email);
		return translateCompanyET(comp_res);
	}
	return ET_SUCCESS;
}



static int calcPrice(int room_faculty,int  client_faculty, int price,
					 int num_ppl){
	int totalPrice = price*num_ppl;
	if ( room_faculty == client_faculty ){
		totalPrice = (3*totalPrice)/4;
	}
	return totalPrice;
}

static Escaper getEscaperByOrder(EscapeTechnion escapeTechnion,
								 Order order, EscapeTechnionResult *result){
	assert(escapeTechnion!=NULL && order!=NULL && result!=NULL);
	*result = ET_SUCCESS;
	Escaper curr_escaper = setGetFirst(escapeTechnion->escapers);
	List escaper_orders;
	while ( curr_escaper!=NULL ){
		EscaperResult e_res = escaperListOrders(curr_escaper, &escaper_orders);
		if ( e_res != ESCAPER_SUCCESS ){
			*result = translateEscaperET(e_res);
			return NULL;
		}
		Order curr_escaper_order = listGetFirst(escaper_orders);
		while ( curr_escaper_order != NULL ){
			if ( compareOrdersForReport(curr_escaper_order, order) == 0 ){
				listDestroy(escaper_orders);
				escaper_orders = NULL;
				return curr_escaper;
			}
			curr_escaper_order = listGetNext(escaper_orders);
		}
		listDestroy(escaper_orders);
		escaper_orders = NULL;
		curr_escaper = setGetNext(escapeTechnion->escapers);
	}
	return NULL;
}

static void removeDayOrders(EscapeTechnion escapeTechnion){
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	while ( escaper != NULL ){
		escaperNextDay(escaper);
		escaperRemoveOldOrders(escaper, 0);
		escaper = setGetNext(escapeTechnion->escapers);
	}
}

static EscapeTechnionResult isEmailTaken(EscapeTechnion escapeTechnion,
									   char* email, bool* taken) {
	Company company = setGetFirst(escapeTechnion->companies);
	char* current_email;
	EscapeTechnionResult res;
	while ( company != NULL ){
		res = translateCompanyET(getCompanyEmail(company, &current_email));
		if ( res != ET_SUCCESS ){
			return res;
		}
		if ( strcmp(email, current_email) == 0 ){
			*taken = true;
			free(current_email);
			return ET_SUCCESS;
		}
		free(current_email);
		company = setGetNext(escapeTechnion->companies);
	}
	Escaper escaper = setGetFirst(escapeTechnion->escapers);
	while ( escaper != NULL ){
		res = translateEscaperET(escaperGetEmail(escaper, &current_email));
		if ( res != ET_SUCCESS ){
			return res;
		}
		if ( strcmp(email, current_email) == 0){
			*taken = true;
			free(current_email);
			return ET_SUCCESS;
		}
		free(current_email);
		escaper = setGetNext(escapeTechnion->escapers);
	}
	*taken = false;
	return ET_SUCCESS;
}

static EscapeTechnionResult areThereRooms(EscapeTechnion escapeTechnion,
										  bool* result){
	if ( escapeTechnion == NULL || result == NULL ){
		return ET_NULL_PARAMETER;
	}
	Company company = setGetFirst(escapeTechnion->companies);
	while ( company != NULL ){
		CompanyResult res;
		bool thereAreRooms;
		res = (companyAreThereRooms(company, &thereAreRooms));
		if ( res == COMP_SUCCESS && thereAreRooms){
			*result = true;
			return ET_SUCCESS;
		}
		company = setGetNext(escapeTechnion->companies);
	}
	*result = false;
	return ET_SUCCESS;
}
