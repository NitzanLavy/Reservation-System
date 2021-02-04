#include <stdio.h>
#include <stdlib.h>
#include "../EscapeTechnion.h"
#include "test_utilities.h"
#include "../mtm_ex3.h"

static EscapeTechnion getExampleEscapeTechnion(){
	EscapeTechnionResult res;
	EscapeTechnion escape_technion = addEscapeTechnion(&res);
	ASSERT_TEST(res==ET_SUCCESS);
	return escape_technion;
}
static bool testAddEscapeTechnion() {
	EscapeTechnionResult res;
	EscapeTechnion escape_technion = addEscapeTechnion(&res);
	ASSERT_TEST(res == ET_SUCCESS);
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testCopyEscapeTechnion() {
	EscapeTechnionResult res;
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnion copy = copyEscapeTechnion(escape_technion, &res);
	ASSERT_TEST(res == ET_SUCCESS && copy != NULL);
	destroyEscapeTechnion(escape_technion);
	destroyEscapeTechnion(copy);
	copy = copyEscapeTechnion(NULL, &res);
	ASSERT_TEST(res == ET_NULL_PARAMETER && copy == NULL);
	return true;
}

static bool testAddCompany() {
	EscapeTechnionResult res;
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	res = addCompany(escape_technion, "legal@email", UNKNOWN-1);
	// all parameters for company are legal
	ASSERT_TEST(res == ET_SUCCESS);
	res = addCompany(escape_technion, "legal@email", UNKNOWN-1);
	// try to add the same company again
	ASSERT_TEST(res == ET_EMAIL_ALREADY_EXISTS);
	res = addCompany(escape_technion, "illegal.email", UNKNOWN-1);
	ASSERT_TEST(res == ET_INVALID_PARAMETER);
	res = addCompany(escape_technion, "legal@email2", UNKNOWN);
	ASSERT_TEST(res == ET_INVALID_PARAMETER);
	res = addCompany(escape_technion, "legal@email3", -1);
	ASSERT_TEST(res == ET_INVALID_PARAMETER);
	res = addCompany(escape_technion, "roomsinc@gmail.com", 13);
	ASSERT_TEST(res == ET_SUCCESS);
	destroyEscapeTechnion(escape_technion);
	return true;
}


static bool testRemoveCompany() {
	EscapeTechnionResult res;
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	res = addCompany(escape_technion, "legal@email", UNKNOWN-1);
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeCompany(escape_technion, "legal@email");
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeCompany(escape_technion, "legal@email");
	// trying to remove a company that was already removed
	ASSERT_TEST(res == ET_COMPANY_EMAIL_DOES_NOT_EXIST);
	res = addCompany(escape_technion, "legal@email", UNKNOWN-1);
	// re-add a company that was removed
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeCompany(escape_technion, NULL);
	ASSERT_TEST(res == ET_NULL_PARAMETER);
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testAddRoom() {
	EscapeTechnionResult res;
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	res = addCompany(escape_technion, "legal@email", UNKNOWN-1);
	ASSERT_TEST(res==ET_SUCCESS);
	res = addRoom(escape_technion, "legal@email", 1, 4, 3, 20, 22, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "legal@email", 1, 4, 3, 20, 22, 9);
	ASSERT_TEST(res == ET_ID_ALREADY_EXIST);
	res = addRoom(escape_technion, NULL, 1, 4, 3, 20, 22, 9);
	ASSERT_TEST(res == ET_NULL_PARAMETER); // null email
	res = addRoom(escape_technion, "legal@email", -1, 4, 3, 20, 22, 9);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // invalid id
	res = addRoom(escape_technion, "legal@email", 1, 5, 3, 20, 22, 9);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // invalid price
	res = addRoom(escape_technion, "legal@email", 1, 4, 0, 20, 22, 9);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // recommended number of people
	// is less than 1
	res = addRoom(escape_technion, "legal@email", 1, 4, 1, 25, 26, 9);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // open_hour is above 23
	res = addRoom(escape_technion, "legal@email", 1, 4, 1, 23, 26, 9);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // close_hour is above 24
	res = addRoom(escape_technion, "legal@email", 1, 4, 1, 23, 23, 9);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // open_hour isn't smaller than
	// close_hour
	res = addRoom(escape_technion, "legal@email", 1, 4, 1, 20, 22, 11);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // difficulty above 10
	res = addRoom(escape_technion, "legal@email", 1, 4, 1, 20, 22, 0);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // difficulty below 1
	res = addRoom(escape_technion, "nomatch@email", 1, 4, 1, 20, 22, 10);
	ASSERT_TEST(res == ET_COMPANY_EMAIL_DOES_NOT_EXIST);
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testRemoveRoom() {
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addCompany(escape_technion, "legal@email", UNKNOWN-1);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "legal@email", 1, 4, 1, 22, 23, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeRoom(escape_technion, UNKNOWN-1, 1);
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeRoom(escape_technion, UNKNOWN-1, 1);
	ASSERT_TEST(res == ET_ID_DOES_NOT_EXIST );
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testAddEscaper() {
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addEscaper(escape_technion, "e1@", BIOLOGY, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addEscaper(escape_technion, "e1@", COMPUTER_SCIENCE, 5);
	ASSERT_TEST(res == ET_EMAIL_ALREADY_EXISTS);
	res = addEscaper(escape_technion, "illegal_email", BIOLOGY, 5);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // email isn't legal
	res = addEscaper(escape_technion, "legal@email", UNKNOWN, 5);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // faculty isn't legal
	res = addEscaper(escape_technion, "legal@email", -1 ,5);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // faculty isn't legal
	res = addEscaper(escape_technion, "legal@email", BIOLOGY, 0);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // skill level < 1
	res = addEscaper(escape_technion, "legal@email", BIOLOGY, 11);
	ASSERT_TEST(res == ET_INVALID_PARAMETER); // skill level > 10)
	res = addEscaper(escape_technion, NULL, BIOLOGY, 5);
	ASSERT_TEST(res == ET_NULL_PARAMETER);
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testRemoveEscaper() {
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addEscaper(escape_technion, "e1@", BIOLOGY, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeEscaper(escape_technion, "e1@");
	ASSERT_TEST(res == ET_SUCCESS);
	res = removeEscaper(escape_technion, "e1@");
	ASSERT_TEST(res == ET_CLIENT_EMAIL_DOES_NOT_EXIST); //trying to remove
	// an escaper that was already removed
	destroyEscapeTechnion(escape_technion);
	return true;

}

static bool testAddOrder() {
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addEscaper(escape_technion, "e1@", BIOLOGY, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addCompany(escape_technion, "c1@", COMPUTER_SCIENCE);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "c1@", 1, 4, 3, 8, 12, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "c1@", 5, 4, 3, 8, 12, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(escape_technion, "a1@", COMPUTER_SCIENCE, 1, 0, 7, 2);
	ASSERT_TEST(res == ET_CLIENT_EMAIL_DOES_NOT_EXIST);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(NULL, "e1@", COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_NULL_PARAMETER);
	res = addOrder(escape_technion, NULL, COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_NULL_PARAMETER);
	res = addOrder(escape_technion, "e1", COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_INVALID_PARAMETER);
	res = addOrder(escape_technion, "@321@", COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_INVALID_PARAMETER);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 2, 0, 8, 2);
	ASSERT_TEST(res == ET_ID_DOES_NOT_EXIST);
	res = addOrder(escape_technion, "e1@5bd", COMPUTER_SCIENCE, 1, 0, 8, 5);
	ASSERT_TEST(res == ET_CLIENT_EMAIL_DOES_NOT_EXIST);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 5, 0, 8, 5);
	ASSERT_TEST(res == ET_CLIENT_IN_ROOM);
	destroyEscapeTechnion(escape_technion);
	return true;

}

static bool testEscaperRecommended() {
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addCompany(escape_technion, "company1@", BIOLOGY);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "company1@", 1, 4, 30, 8, 12, 5);
	// id = 1, faculty = 10 ,rec_num_ppl = 30, difficulty = 5
	ASSERT_TEST(res == ET_SUCCESS);
	res = addEscaper(escape_technion, "e1@", BIOLOGY, 5);
	// faculty = 10, skill = 5
	ASSERT_TEST(res == ET_SUCCESS);
	res = escaperRecommended(escape_technion, "e1@", 3);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(escape_technion, "e1@", BIOLOGY, 1, 0, 8, 3);
	ASSERT_TEST(res == ET_CLIENT_IN_ROOM);
	res = addRoom(escape_technion, "company1@", 2, 4, 2, 8, 12, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = escaperRecommended(escape_technion, "e1@", 3);
	ASSERT_TEST(res == ET_SUCCESS);
	ASSERT_TEST(escaperRecommended(escape_technion, "illegal_mail", 7)==
				ET_INVALID_PARAMETER);
	ASSERT_TEST(escaperRecommended(NULL, "legal@email", 8) ==
				ET_NULL_PARAMETER);
	ASSERT_TEST(escaperRecommended(escape_technion, "legal@email", -1) ==
				ET_INVALID_PARAMETER); // illegal num_ppl
	res = addCompany(escape_technion, "company2@", COMPUTER_SCIENCE);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "company2@", 3, 24, 2, 9, 12, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "company2@", 3, 4, 8, 10, 24, 5);
	ASSERT_TEST(res == ET_ID_ALREADY_EXIST);
	res = escaperRecommended(escape_technion, "e1@", 3);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addEscaper(escape_technion, "e2@", COMPUTER_SCIENCE, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = escaperRecommended(escape_technion, "e2@", 2);
	ASSERT_TEST(res == ET_SUCCESS);
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testReportDay(){
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addEscaper(escape_technion, "e1@", BIOLOGY, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addCompany(escape_technion, "c1@", COMPUTER_SCIENCE);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "c1@", 1, 4, 3, 8, 12, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "c1@", 5, 4, 3, 8, 12, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(escape_technion, "a1@", COMPUTER_SCIENCE, 1, 0, 7, 2);
	ASSERT_TEST(res == ET_CLIENT_EMAIL_DOES_NOT_EXIST);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 1, 2, 8, 2);
	ASSERT_TEST(res == ET_SUCCESS);
	res = reportDay(escape_technion, stdout);
	ASSERT_TEST(res == ET_SUCCESS);
	destroyEscapeTechnion(escape_technion);
	return true;
}

static bool testReportBestFaculties(){
	EscapeTechnion escape_technion = getExampleEscapeTechnion();
	EscapeTechnionResult res;
	res = addEscaper(escape_technion, "e1@", BIOLOGY, 5);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addCompany(escape_technion, "c1@", COMPUTER_SCIENCE);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "c1@", 1, 4, 3, 8, 12, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addRoom(escape_technion, "c1@", 5, 4, 3, 8, 12, 9);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(escape_technion, "a1@", COMPUTER_SCIENCE, 1, 0, 7, 2);
	ASSERT_TEST(res == ET_CLIENT_EMAIL_DOES_NOT_EXIST);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 1, 0, 8, 2);
	ASSERT_TEST(res == ET_SUCCESS);
	res = addOrder(escape_technion, "e1@", COMPUTER_SCIENCE, 1, 2, 8, 2);
	ASSERT_TEST(res == ET_SUCCESS);
	res = reportDay(escape_technion, stdout);
	ASSERT_TEST(res == ET_SUCCESS);
	reportBestFaculties(escape_technion, stdout);
	destroyEscapeTechnion(escape_technion);
	return true;
}

int main(){
	RUN_TEST(testAddEscapeTechnion);
	RUN_TEST(testCopyEscapeTechnion);
	RUN_TEST(testAddCompany);
	RUN_TEST(testRemoveCompany);
	RUN_TEST(testAddRoom);
	RUN_TEST(testRemoveRoom);
	RUN_TEST(testAddEscaper);
	RUN_TEST(testAddOrder);
	RUN_TEST(testEscaperRecommended);
	RUN_TEST(testRemoveEscaper);
	RUN_TEST(testReportDay);
	RUN_TEST(testReportBestFaculties);
	return 0;
}
