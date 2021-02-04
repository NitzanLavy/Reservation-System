

#include "test_utilities.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../company.h"
#include "../mtm_ex3.h"

static Company getExampleCompany() {
	CompanyResult res;
	Company company = companyCreate("Atest@email", UNKNOWN-1, &res);
	ASSERT_TEST(res==COMP_SUCCESS);
	return company;
}
static bool testCompanyCreate() {
	CompanyResult res;
	Company company = getExampleCompany();
	ASSERT_TEST(company!=NULL);
	companyDestroy(company);
	company = companyCreate("illegalemail", UNKNOWN-1, &res);
	ASSERT_TEST(res==COMP_INVALID_PARAMETER && company==NULL);
	companyDestroy(company);
	company = companyCreate("test@email", UNKNOWN, &res);
	ASSERT_TEST(res==COMP_INVALID_PARAMETER && company==NULL);
	companyDestroy(company);
	company = companyCreate("test@email", -1, &res);
	ASSERT_TEST(res==COMP_INVALID_PARAMETER && company==NULL);
	companyDestroy(company);
	company = companyCreate(NULL, UNKNOWN-1, &res);
	ASSERT_TEST(res==COMP_NULL_PARAMETER && company==NULL);
	companyDestroy(company);
	company = companyCreate(NULL, UNKNOWN, &res);
	ASSERT_TEST(res==COMP_NULL_PARAMETER && company==NULL);
	companyDestroy(company);
	return true;
}

static bool testCompanyCompare() {
	Company company1 = getExampleCompany();
	Company company2 = getExampleCompany();
	// both companies have the same email- "Atest@email"
	ASSERT_TEST(companyCompare(company1,company2) == 0);
	companyDestroy(company2);
	CompanyResult res;
	company2 = companyCreate("Bother@email", UNKNOWN-1, &res);
	ASSERT_TEST(res==COMP_SUCCESS);
	ASSERT_TEST( companyCompare(company1,company2)<0 );
	ASSERT_TEST( companyCompare(company2, company1)>0 );
	companyDestroy(company2);
	companyDestroy(company1);
	return true;
}

static bool testCompanyCopy() {
	CompanyResult res;
	Company company1 = companyCreate("test@email", BIOLOGY, &res);
	ASSERT_TEST(res==COMP_SUCCESS);
	Company company2 = companyCopy(company1, &res);
	ASSERT_TEST(res==COMP_SUCCESS);
	ASSERT_TEST(companyCompare(company1, company2) == 0);
	companyDestroy(company2);
	company2 = companyCopy(NULL, &res);
	ASSERT_TEST(res == COMP_NULL_PARAMETER && company2==NULL);
	companyDestroy(company2);
	companyDestroy(company1);
	return true;
}

static bool testAddRoomToCompany() {
	CompanyResult res;
	Company company = getExampleCompany();
	res = addRoomToCompany(company, 1, 16, 2, 22, 23, 9);
	ASSERT_TEST(res == COMP_SUCCESS);
	res = addRoomToCompany(company, 1, 16, 2, 22, 23, 9);
	// attempting to add the same room
	ASSERT_TEST(res == COMP_ID_ALREADY_EXISTS);
	res = addRoomToCompany(company, -1, 16, 2, 22, 23, 9);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // id below 1
	res = addRoomToCompany(company, 1, 15, 2, 22, 23, 9);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // price isn't divisible by 4
	res = addRoomToCompany(company, 1, 16, 0, 22, 23, 9);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // recommended number of people
	// is below 1
	res = addRoomToCompany(company, 1, 16, 2, 26, 27, 9);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // open hour over 23
	res = addRoomToCompany(company, 1, 16, 2, 22, 25, 9);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // close hour over 24
	res = addRoomToCompany(company, 1, 16, 2, 22, 21, 9);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // close hour is smaller than
	// the open hour
	res = addRoomToCompany(company, 1, 16, 2, 22, 23, 12);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // difficulty over 10
	res = addRoomToCompany(company, 1, 15, 2, 22, 23, 0);
	ASSERT_TEST(res == COMP_INVALID_PARAMETER); // difficulty below 1
	companyDestroy(company);
	return true;
}

static bool testRemoveRoomFromCompany() {
	CompanyResult res;
	Company company = getExampleCompany();
	res = addRoomToCompany(company, 1, 16, 2, 22, 24, 9);
	ASSERT_TEST(res==COMP_SUCCESS); // make sure there was no memory problem
	res = removeRoomFromCompany(company, 1);
	ASSERT_TEST(res == COMP_SUCCESS);
	res = addRoomToCompany(company, 1, 16, 2, 22, 24, 9);
	// should be able to add the same room again
	ASSERT_TEST(res == COMP_SUCCESS);
	res = removeRoomFromCompany(company, 2);
	// attempt to remove a room that wasn't added
	ASSERT_TEST(res == COMP_ID_DOES_NOT_EXIST);
	companyDestroy(company);
	return true;
}

static bool testGetCompanyEmail(){
	CompanyResult res;
	Company company = companyCreate("check@email", UNKNOWN-1, &res);
	ASSERT_TEST(res == COMP_SUCCESS);
	char* comp_email;
	res = getCompanyEmail(company, &comp_email);
	ASSERT_TEST(res == COMP_SUCCESS && strcmp(comp_email,"check@email")==0);
	res = getCompanyEmail(NULL, &comp_email);
	ASSERT_TEST(res == COMP_NULL_PARAMETER);
	res = getCompanyEmail(company, NULL);
	ASSERT_TEST(res == COMP_NULL_PARAMETER);
	res = getCompanyEmail(NULL, NULL);
	ASSERT_TEST(res == COMP_NULL_PARAMETER);
	free(comp_email);
	companyDestroy(company);
	return true;
}

static bool testIsRoomInCompany() {
	Company company = getExampleCompany();
	CompanyResult comp_res;
	comp_res = addRoomToCompany(company, 1, 16, 2, 22, 24, 9);
	ASSERT_TEST(comp_res==COMP_SUCCESS);
	bool res;
	comp_res = isRoomInCompany(company, 1, &res);
	ASSERT_TEST(res == true);
	comp_res = removeRoomFromCompany(company, 1);
	comp_res = isRoomInCompany(company, 1, &res);
	ASSERT_TEST(res == false);
	companyDestroy(company);
	return true;
}

static bool testGetRecommendedRoom() {
	Company company = getExampleCompany();
	CompanyResult comp_res;
	comp_res = addRoomToCompany(company, 4, 16, 7, 2, 24, 9);
	// recommended number of people:7 difficulty:9 id:4
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	comp_res = addRoomToCompany(company, 5, 20, 1, 3, 17, 3);
	// recommended number of people:1 difficulty:3 id:5
	int rec_id, rec_score;
	comp_res = getRecommendedRoom(company, 7, 9, &rec_id, &rec_score);
	ASSERT_TEST(comp_res == COMP_SUCCESS && rec_id == 4 && rec_score == 0);
	comp_res = getRecommendedRoom(company, 1, 3, &rec_id, &rec_score);
	ASSERT_TEST(comp_res == COMP_SUCCESS && rec_id == 5 && rec_score == 0);
	comp_res = getRecommendedRoom(company, 1, 4, &rec_id, &rec_score);
	ASSERT_TEST(comp_res == COMP_SUCCESS && rec_id == 5 && rec_score == 1);
	comp_res = getRecommendedRoom(NULL, 1, 4, &rec_id, &rec_score);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getRecommendedRoom(company, 1, 4, NULL, &rec_score);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getRecommendedRoom(company, 1, 4, &rec_id, NULL);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getRecommendedRoom(company, 0, 4, &rec_id, &rec_score);
	// illegal number of people (below 1)
	ASSERT_TEST(comp_res == COMP_INVALID_PARAMETER);
	comp_res = getRecommendedRoom(company, 1, 0, &rec_id, &rec_score);
	// illegal skill level (below 1)
	ASSERT_TEST(comp_res == COMP_INVALID_PARAMETER);
	companyDestroy(company);
	return true;
}

static bool testGetCompanyFaculty(){
	CompanyResult comp_res;
	Company company = companyCreate("legal@", UNKNOWN-1, &comp_res);
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	int faculty;
	comp_res = getCompanyFaculty(company, &faculty);
	ASSERT_TEST(comp_res == COMP_SUCCESS && faculty == UNKNOWN-1);
	comp_res = getCompanyFaculty(NULL, &faculty);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getCompanyFaculty(company, NULL);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	companyDestroy(company);
	return true;
}

static bool testGetCompanyRoomDifficulty(){
	CompanyResult comp_res;
	Company company = companyCreate("legal@", UNKNOWN-1, &comp_res);
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	comp_res = addRoomToCompany(company, 5, 16, 5, 5, 7, 9); //id = 5
	// difficulty = 9
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	int diff;
	comp_res = getCompanyRoomDifficulty(company, 5, &diff);
	ASSERT_TEST(comp_res == COMP_SUCCESS && diff == 9);
	comp_res = getCompanyRoomDifficulty(company, -7, &diff);
	ASSERT_TEST(comp_res == COMP_INVALID_PARAMETER); // illegal id
	comp_res = getCompanyRoomDifficulty(NULL, 5, &diff);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getCompanyRoomDifficulty(company, 5, NULL);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	companyDestroy(company);
	return true;
}

static bool testGetCompanyRoomPrice(){
	CompanyResult comp_res;
	Company company = companyCreate("legal@", UNKNOWN-1, &comp_res);
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	comp_res = addRoomToCompany(company, 5, 16, 5, 5, 7, 9); //id = 5
	// price = 16
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	int price;
	comp_res = getCompanyRoomPrice(company, 5, &price);
	ASSERT_TEST(comp_res == COMP_SUCCESS && price == 16);
	comp_res = getCompanyRoomPrice(company, -7, &price);
	ASSERT_TEST(comp_res == COMP_INVALID_PARAMETER); // illegal id
	comp_res = getCompanyRoomPrice(NULL, 5, &price);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getCompanyRoomPrice(company, 5, NULL);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	companyDestroy(company);
	return true;
}

static bool testGetCompanyRoomOpenHours(){
	CompanyResult comp_res;
	Company company = companyCreate("legal@", UNKNOWN-1, &comp_res);
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	comp_res = addRoomToCompany(company, 5, 16, 5, 5, 7, 9); //id = 5
	// open = 5 close = 7
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	int open, close;
	comp_res = getCompanyRoomOpenHours(company, 5, &open, &close);
	ASSERT_TEST(comp_res == COMP_SUCCESS && open == 5 && close == 7);
	comp_res = getCompanyRoomOpenHours(company, -7, &open, &close);
	ASSERT_TEST(comp_res == COMP_INVALID_PARAMETER); // illegal id
	comp_res = getCompanyRoomOpenHours(NULL, 5, &open, &close);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getCompanyRoomOpenHours(company, 5, NULL, &close);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	comp_res = getCompanyRoomOpenHours(company, 5, &open, NULL);
	ASSERT_TEST(comp_res == COMP_NULL_PARAMETER);
	companyDestroy(company);
	return true;
}

static bool testCompanyAreThereRooms(){
	CompanyResult comp_res;
	bool thereAreRooms;
	Company company = getExampleCompany();
	comp_res = companyAreThereRooms(company, &thereAreRooms);
	ASSERT_TEST(comp_res == COMP_SUCCESS && !thereAreRooms);
	comp_res = addRoomToCompany(company, 5, 16, 5, 5, 7, 9);
	ASSERT_TEST(comp_res == COMP_SUCCESS);
	comp_res = companyAreThereRooms(company, &thereAreRooms);
	ASSERT_TEST(comp_res == COMP_SUCCESS && thereAreRooms);
	companyDestroy(company);
	return true;

}
int main(){
	RUN_TEST(testCompanyCreate);
	RUN_TEST(testCompanyCompare);
	RUN_TEST(testCompanyCopy);
	RUN_TEST(testAddRoomToCompany);
	RUN_TEST(testRemoveRoomFromCompany);
	RUN_TEST(testGetCompanyEmail);
	RUN_TEST(testIsRoomInCompany);
	RUN_TEST(testGetRecommendedRoom);
	RUN_TEST(testGetCompanyFaculty);
	RUN_TEST(testGetCompanyRoomDifficulty);
	RUN_TEST(testGetCompanyRoomPrice);
	RUN_TEST(testGetCompanyRoomOpenHours);
	RUN_TEST(testCompanyAreThereRooms);
	return 0;
}


