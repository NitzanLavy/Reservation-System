
#include "company.h"
#include <string.h>
#include "mtm_ex3.h"
#include "set.h"
#include "room.h"
#include <stdbool.h>


struct company_s{
	char *email;
	int faculty;
	Set rooms;
};

/* receives a SetResult type and returns the equivalent CompanyResult value */
static CompanyResult convertSetResultToCompResult(SetResult set_res);


/* receives a RoomResult type and returns the equivalent CompanyResult value */
static CompanyResult convertRoomResultToCompResult(RoomResult room_res);


/* a generic copy function to be used when creating the company's rooms set */
static void* genericRoomCopy(void* room);


/* a generic destroy function to be used when creating the company's rooms set*/
static void genericRoomDestroy(void* room);


/* a generic compare function to be used when creating the comapny's rooms set*/
static int genericRoomCompare(void* room1, void* room2);


/* receives a recommended number of people, number of people making an order,
 * difficulty and skill_level and return the recommendation score
 */
static int calcRecommendedScore(int rec_num_ppl, int num_ppl,
								int difficulty, int skill_level);


/* receives a number and returns it squared */
static int getSquared(int num);


/* receives the current best score and id, the faculty a room, the number of
 * people an order is for and the escaper's skill_level
 * compares between the current score and the best score and updates the best
 * score and id if needed */
static void updateBestScore(int* best_score, int* best_id, int faculty,
							Room room, int num_ppl, int skill_level);


/* returns a room in the company with the given room_id.
 * result is set to COMPANY_ID_DOES_NOT_EXIST if the room isn't found
 */
static Room getRoomById(Company company, int room_id, CompanyResult *res);


/*
 * This function converts SetResult type messages to CompanyResult messages.
 */
static CompanyResult convertSetResultToCompResult(SetResult set_res){
	switch (set_res) {
		case SET_SUCCESS:
			return COMP_SUCCESS;
		case SET_OUT_OF_MEMORY:
			return COMP_OUT_OF_MEMORY;
		case SET_NULL_ARGUMENT:
			return COMP_NULL_PARAMETER;
		case SET_ITEM_ALREADY_EXISTS:
			return COMP_ID_ALREADY_EXISTS;
		case SET_ITEM_DOES_NOT_EXIST:
			return COMP_ID_DOES_NOT_EXIST;
	}
	return COMP_SUCCESS;
}

/*
 * This function converts RoomResult type messages to CompanyResult messages.
 */
static CompanyResult convertRoomResultToCompResult(RoomResult room_res){
	switch (room_res) {
		case ROOM_SUCCESS:
			return COMP_SUCCESS;
		case ROOM_OUT_OF_MEMORY:
			return COMP_OUT_OF_MEMORY;
		case ROOM_NULL_PARAMETER:
			return COMP_NULL_PARAMETER;
		case ROOM_INVALID_PARAMETER:
			return COMP_INVALID_PARAMETER;
	}
	return COMP_SUCCESS;
}


Company companyCreate(char* email, int faculty, CompanyResult *result){
	assert(result!=NULL);
	Company new_company = malloc(sizeof(*new_company));
	if ( new_company == NULL ){
		*result = COMP_OUT_OF_MEMORY;
		return NULL;
	}
	if ( email == NULL ){
		*result = COMP_NULL_PARAMETER;
		free(new_company);
		return NULL;
	}
	if (!isLegalFaculty(faculty) || !isLegalEmail(email)) {
		*result = COMP_INVALID_PARAMETER;
		free(new_company);
		return NULL;
	}
	char *company_email = malloc(strlen(email)+1);
	if ( company_email == NULL){
		free(company_email);
		free(new_company);
		*result = COMP_OUT_OF_MEMORY;
		return NULL;
	}
	new_company->rooms = setCreate(genericRoomCopy, genericRoomDestroy,
									   genericRoomCompare);
	if (new_company->rooms == NULL) {
		companyDestroy(new_company);
		free(company_email);
		*result = COMP_OUT_OF_MEMORY;
		return NULL;
	}
	new_company->email = company_email;
	strcpy(new_company->email, email);
	if ( !isLegalFaculty(faculty) || !isLegalEmail(new_company->email) ){
		*result = COMP_INVALID_PARAMETER;
		companyDestroy(new_company);
		free(company_email);
		return NULL;
	}
	new_company->faculty = faculty;
	*result = COMP_SUCCESS;
	return new_company;
}


static void* genericRoomCopy(void* room){
	assert (room != NULL);
	Room original_room = (Room)room;
	RoomResult result;
	Room copied_room = roomCopy(original_room, &result);
	return copied_room;
}


static void genericRoomDestroy(void* room){
	roomDestroy((Room)room);
}


static int genericRoomCompare(void* room1, void* room2){
	assert (room1 != NULL && room2 != NULL);
	return roomCompare(room1, room2);
}


Company companyCopy(Company company, CompanyResult *result){
	assert(result!=NULL);
	if (company == NULL){
		*result = COMP_NULL_PARAMETER;
		return NULL;
	}
	Company new_comp = companyCreate(company->email, company->faculty, result);
	return new_comp;
}


void companyDestroy(Company company){
	if ( company == NULL ){
		return;
	}
	setDestroy(company->rooms);
	free(company->email);
	free(company);
	return;
}


CompanyResult addRoomToCompany(Company company, int room_id, int price,
							   int rec_num_ppl, int open_hour, int close_hour,
							   int difficulty){
	if ( company == NULL ){
		return COMP_NULL_PARAMETER;
	}
	if (!isLegalId(room_id) || !isLegalPrice(price) ||
			!isLegalNumPpl(rec_num_ppl) ||
			!isLegalOpeningHours(open_hour,close_hour) ||
			!isLegalDifficulty(difficulty)) {
		return COMP_INVALID_PARAMETER;
	}
	RoomResult room_res;
	Room room = roomCreate(room_id, price, rec_num_ppl, open_hour, close_hour,
						   difficulty, &room_res);
	CompanyResult comp_res = convertRoomResultToCompResult(room_res);
	if ( comp_res != COMP_SUCCESS ){
		return comp_res;
	}
	SetResult set_res = setAdd(company->rooms, room);
	comp_res = convertSetResultToCompResult(set_res);
	roomDestroy(room);
	return comp_res;
}


CompanyResult removeRoomFromCompany(Company company, int room_id){
	if ( company == NULL ){
		return COMP_NULL_PARAMETER;
	}
	if (!isLegalId(room_id)) {
		return COMP_INVALID_PARAMETER;
	}
	Room room = setGetFirst(company->rooms);
	while ( room!=NULL ){
		int curr_id;
		getRoomId(room, &curr_id);
		if ( curr_id == room_id ){
			break;
		}
		room = setGetNext(company->rooms);
	}
	if ( room == NULL ){
		return COMP_ID_DOES_NOT_EXIST;
	}
	SetResult set_res = setRemove(company->rooms, room);
	CompanyResult comp_res = convertSetResultToCompResult(set_res);
	return comp_res;
}


int companyCompare(Company company1, Company company2){
	if ( company1 == NULL || company1->email == NULL
		 || company2 == NULL || company2->email == NULL ){
		return 0;
	}
	return strcmp(company1->email, company2->email);
}


CompanyResult getCompanyEmail(Company company, char** email){
	if ( company == NULL || company->email == NULL || email == NULL ){
		return COMP_NULL_PARAMETER;
	}
	char* copy_email = malloc(strlen(company->email)+1);
	if ( copy_email == NULL ){
		return COMP_OUT_OF_MEMORY;
	}
	strcpy(copy_email, company->email);
	*email = copy_email;
	return COMP_SUCCESS;
}


CompanyResult isRoomInCompany(Company company, int id, bool* result){
	if ( company == NULL || result == NULL ){
		return COMP_NULL_PARAMETER;
	}
	if (!isLegalId(id)) {
		return COMP_INVALID_PARAMETER;
	}
	Room current_room = setGetFirst(company->rooms);
	while ( current_room != NULL ){
		int curr_id;
		getRoomId(current_room, &curr_id);
		if ( id == curr_id ) {
			*result = true;
			return COMP_SUCCESS;
		}
		current_room = setGetNext(company->rooms);
	}
	*result = false;
	return COMP_SUCCESS;
}


CompanyResult getRecommendedRoom(Company company, int num_ppl, int skill_level,
								 int *rec_id, int *rec_score){
	if ( company == NULL || rec_id == NULL || rec_score == NULL){
		return COMP_NULL_PARAMETER;
	}
	if (!isLegalNumPpl(num_ppl) || !isLegalSkillLevel(skill_level)){
		return COMP_INVALID_PARAMETER;
	}
	CompanyResult comp_res = COMP_NO_ROOMS_AVAILABLE;
	Room room = setGetFirst(company->rooms);
	if ( room == NULL ){
		return comp_res;
	}
	int best_score, best_id, rec_num_ppl, difficulty;
	RoomResult r_res;
	r_res = getRoomDifficulty(room, &difficulty);
	if ( r_res != ROOM_SUCCESS ){
		return COMP_INVALID_PARAMETER;
	}
	r_res = getRecNumPpl(room, &rec_num_ppl);
	if ( convertRoomResultToCompResult(r_res) != COMP_SUCCESS ){
		return convertRoomResultToCompResult(r_res);
	}
	best_score = calcRecommendedScore(rec_num_ppl, num_ppl, difficulty,
										 skill_level);
	r_res = getRoomId(room, &best_id);
	if ( r_res != ROOM_SUCCESS ){
		return COMP_INVALID_PARAMETER;
	}
	*rec_score = best_score;
	*rec_id = best_id;
	room = setGetNext(company->rooms);
	while ( room!=NULL ){
		updateBestScore(&best_score, &best_id, company->faculty, room,
						num_ppl, skill_level);
		room = setGetNext(company->rooms);
	}
	*rec_score = best_score;
	*rec_id = best_id;
	comp_res = COMP_SUCCESS;
	return comp_res;
}


static void updateBestScore(int* best_score, int* best_id, int faculty,
							Room room, int num_ppl, int skill_level){
	assert(best_score!=NULL && best_id!=NULL && room!=NULL);
	int current_id;
	getRoomId(room, &current_id);
	int rec_num_ppl;
	RoomResult room_res = getRecNumPpl(room, &rec_num_ppl);
	CompanyResult res = convertRoomResultToCompResult(room_res);
	if ( res != COMP_SUCCESS ){
		return;
	}
	int difficulty;
	getRoomDifficulty(room, &difficulty);
	int current_score = calcRecommendedScore(rec_num_ppl, num_ppl, difficulty,
										     skill_level);
	if ( current_score <= *best_score )
	{
		if ( current_score < *best_score || current_id < *best_id ){
			*best_score = current_score;
			*best_id = current_id;
			return;
		}
	}
	return;
}


static int calcRecommendedScore(int rec_num_ppl, int num_ppl,
								int difficulty, int skill_level){
	return getSquared(rec_num_ppl-num_ppl) + getSquared(difficulty-skill_level);
}


static int getSquared(int num){
	return num*num;
}


CompanyResult getCompanyFaculty(Company company, int *faculty){
	if ( company == NULL || faculty == NULL ){
		return COMP_NULL_PARAMETER;
	}
	*faculty = company->faculty;
	return COMP_SUCCESS;
}


CompanyResult getCompanyRoomDifficulty(Company company, int room_id,
								       int* difficulty){
	if ( company == NULL || difficulty == NULL ){
		return COMP_NULL_PARAMETER;
	}
	if (!isLegalId(room_id)) {
		return COMP_INVALID_PARAMETER;
	}
	CompanyResult res;
	Room room = getRoomById(company, room_id, &res);
	if ( res != COMP_SUCCESS ){
		return res;
	}
	int room_difficulty;
	RoomResult r_res = getRoomDifficulty(room, &room_difficulty);
	if ( r_res != ROOM_SUCCESS ){
		return convertRoomResultToCompResult(r_res);
	}
	*difficulty = room_difficulty;
	return COMP_SUCCESS;
}


CompanyResult getCompanyRoomPrice(Company company,int room_id, int *price){
	if ( company == NULL || price == NULL ){
		return COMP_NULL_PARAMETER;
	}
	if (!isLegalId(room_id)) {
		return COMP_INVALID_PARAMETER;
	}
	CompanyResult res;
	Room room = getRoomById(company, room_id, &res);
	if ( res != COMP_SUCCESS ){
		return res;
	}
	int room_price;
	RoomResult r_res = getRoomPrice(room, &room_price);
	if ( r_res != ROOM_SUCCESS ){
		return convertRoomResultToCompResult(r_res);
	}
	*price = room_price;
	return COMP_SUCCESS;
}


static Room getRoomById(Company company, int room_id, CompanyResult *res){
	assert(res!=NULL);
	if ( company == NULL ){
		*res = COMP_NULL_PARAMETER;
		return NULL;
	}
	if (!isLegalId(room_id)) {
		*res = COMP_INVALID_PARAMETER;
		return NULL;
	}
	Room curr_room = setGetFirst(company->rooms);
	while ( curr_room != NULL ){
		int curr_id;
		RoomResult r_res = getRoomId(curr_room, &curr_id);
		if ( r_res != ROOM_SUCCESS ){
			*res = convertRoomResultToCompResult(r_res);
			return NULL;
		}
		if ( curr_id == room_id ){
			*res = COMP_SUCCESS;
			return curr_room;
		}
		curr_room = setGetNext(company->rooms);
	}
	*res = COMP_ID_DOES_NOT_EXIST;
	return NULL;
}

CompanyResult getCompanyRoomOpenHours(Company company, int room_id,
									  int* open_hour, int *close_hour){
	CompanyResult res;
	if (company==NULL || open_hour==NULL || close_hour==NULL){
		return COMP_NULL_PARAMETER;
	}
	if ( !isLegalId(room_id) ){
		return COMP_INVALID_PARAMETER;
	}
	Room room = getRoomById(company, room_id, &res);
	if ( res!=COMP_SUCCESS ){
		return res;
	}
	int open, close;
	RoomResult room_res = getRoomOpenHours(room, &open, &close);
	res = convertRoomResultToCompResult(room_res);
	if ( res != COMP_SUCCESS ){
		return res;
	}
	*open_hour = open;
	*close_hour = close;
	return COMP_SUCCESS;
}

CompanyResult companyAreThereRooms(Company company, bool* result){
	if ( company == NULL || result == NULL ){
		return COMP_NULL_PARAMETER;
	}
	Room room = setGetFirst(company->rooms);
	*result = ( room!= NULL );
	return COMP_SUCCESS;
}
