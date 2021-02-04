#include <stdlib.h>
#include <stdio.h>
#include "../mtm_ex3.h"
#include "../escaper.h"
#include "test_utilities.h"
#include "../list.h"
#include <string.h>


static Escaper getExampleEscaper(){
	EscaperResult e_res;
	Escaper escaper = escaperCreate("legal@email", BIOLOGY, 5, &e_res);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	return escaper;
}
static bool testEscaperCreate(){
	EscaperResult e_res;
	Escaper escaper = escaperCreate("legal@email", BIOLOGY, 5, &e_res);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	escaperDestroy(escaper);
	escaper = escaperCreate("illegal_email", BIOLOGY, 5, &e_res);
	ASSERT_TEST(e_res == ESCAPER_INVALID_PARAMETER);
	escaperDestroy(escaper);
	escaper = escaperCreate("legal@email", UNKNOWN, 5, &e_res);
	ASSERT_TEST(e_res == ESCAPER_INVALID_PARAMETER);
	escaperDestroy(escaper);
	escaper = escaperCreate("legal@email", BIOLOGY, 0, &e_res);
	ASSERT_TEST(e_res == ESCAPER_INVALID_PARAMETER); // skill level <1
	escaperDestroy(escaper);
	escaper = escaperCreate("legal@email", BIOLOGY, 11, &e_res);
	ASSERT_TEST(e_res == ESCAPER_INVALID_PARAMETER); // skill level > 10
	escaperDestroy(escaper);
	escaper = escaperCreate(NULL, BIOLOGY, 5, &e_res);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER); // NULL email
	escaperDestroy(escaper);
	escaper = escaperCreate(NULL, UNKNOWN, 5, &e_res);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER); // NULL email AND illegal
	// faculty
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperCompare() {
	Escaper escaper = getExampleEscaper();
	Escaper escaper2 = getExampleEscaper(); // both escapers are the same
	ASSERT_TEST(escaperCompare(escaper, escaper2)==0);
	escaperDestroy(escaper);
	escaperDestroy(escaper2);
	return true;
}

static bool testEscaperCopy() {
	Escaper escaper = getExampleEscaper();
	EscaperResult e_res;
	Escaper escaper2 = escaperCopy(escaper, &e_res);
	ASSERT_TEST(e_res = ESCAPER_SUCCESS
			    && escaperCompare(escaper, escaper2)==0);
	escaperDestroy(escaper2);
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperGetEmail() {
	EscaperResult e_res;
	Escaper escaper = escaperCreate("e1@", UNKNOWN-1, 7, &e_res);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	char* email;
	e_res = escaperGetEmail(escaper, &email);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS && strcmp(email, "e1@")==0);
	e_res = escaperGetEmail(NULL, &email);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	e_res = escaperGetEmail(escaper, NULL);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	free(email);
	return true;
}

static bool testEscaperGetFaculty() {
	EscaperResult e_res;
	Escaper escaper = escaperCreate("e1@", UNKNOWN-1, 7, &e_res);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	int faculty;
	e_res = escaperGetFaculty(escaper, &faculty);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS && faculty==UNKNOWN-1);
	e_res = escaperGetFaculty(NULL, &faculty);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	e_res = escaperGetFaculty(escaper, NULL);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperGetSkillLevel(){
	EscaperResult e_res;
	Escaper escaper = escaperCreate("e1@", UNKNOWN-1, 7, &e_res);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	int skill;
	e_res = escaperGetSkillLevel(escaper, &skill);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS && skill==7);
	e_res = escaperGetSkillLevel(NULL, &skill);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	e_res = escaperGetSkillLevel(escaper, NULL);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperNextDay(){
	Escaper escaper = getExampleEscaper();
	EscaperResult e_res = escaperNextDay(escaper);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	e_res = escaperAddOrder(escaper, BIOLOGY, 3, 7, 8, 3);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	e_res = escaperNextDay(escaper);
	ASSERT_TEST(e_res == ESCAPER_SUCCESS);
	e_res = escaperNextDay(NULL);
	ASSERT_TEST(e_res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}


static bool testEscaperAddOrder() {
	Escaper escaper = getExampleEscaper();
	EscaperResult res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 0, 8, 3);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, BIOLOGY, 3, 7, 8, 3);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 0, 8, 3);
	ASSERT_TEST(res == ESCAPER_CLIENT_IN_ROOM); // the same order
	res = escaperAddOrder(escaper, UNKNOWN, 4, 0, 8, 3);
	ASSERT_TEST(res == ESCAPER_INVALID_PARAMETER); // illegal faculty
	res = escaperAddOrder(escaper, BIOLOGY, -1, 0, 8, 3);
	ASSERT_TEST(res == ESCAPER_INVALID_PARAMETER); // illegal id
	res = escaperAddOrder(escaper, BIOLOGY, 5, -1, 8, 3); // illegal day
	ASSERT_TEST(res == ESCAPER_INVALID_PARAMETER);
	res = escaperAddOrder(escaper, BIOLOGY, 6, 0, 24, 25); // illegal open_hour
	ASSERT_TEST(res == ESCAPER_INVALID_PARAMETER);
	res = escaperAddOrder(NULL, BIOLOGY, 9, 0, 11, 7);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperRemoveOldOrders() {
	Escaper escaper = getExampleEscaper();
	EscaperResult res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 5, 8, 3);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 5, 8, 3);
	ASSERT_TEST(res == ESCAPER_CLIENT_IN_ROOM);
	res = escaperRemoveOldOrders(escaper, 6);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 5, 8, 3);
	ASSERT_TEST(res == ESCAPER_SUCCESS); // trying to re-add an order
	// that was removed
	res = escaperRemoveOldOrders(escaper, 0);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 5, 8, 3);
	ASSERT_TEST(res == ESCAPER_CLIENT_IN_ROOM); // make sure the order
	// wasn't removed after escaperRemoveOldOrders
	res = escaperRemoveOldOrders(NULL, 6);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperMaximalRoomId() {
	Escaper escaper = getExampleEscaper();
	EscaperResult res = escaperAddOrder(escaper, BIOLOGY, 3, 7, 8, 3); // id 7
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 1, 8, 3); // 2
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, BIOLOGY, 9, 0, 8, 5); // id 0
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	int maximal;
	res = escaperMaximalRoomId(escaper, &maximal);
	ASSERT_TEST(res == ESCAPER_SUCCESS && maximal == 9);
	res = escaperMaximalRoomId(NULL, &maximal);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	res = escaperMaximalRoomId(escaper, NULL);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

static bool testSearchMatchingOrder() {
	Escaper escaper = getExampleEscaper();
	EscaperResult res = escaperAddOrder(escaper, BIOLOGY, 2, 7, 8, 3); // id 2
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	bool found;
	res = searchMatchingOrder(escaper, BIOLOGY, 2, &found);
	ASSERT_TEST(res == ESCAPER_SUCCESS && found == true);
	res = searchMatchingOrder(escaper, COMPUTER_SCIENCE, 2, &found);
	// same id, different faculty
	ASSERT_TEST(res == ESCAPER_SUCCESS && found == false);
	res = searchMatchingOrder(escaper, BIOLOGY, 3, &found);
	// same faculty, different id
	ASSERT_TEST(res == ESCAPER_SUCCESS && found == false);
	res = searchMatchingOrder(NULL, BIOLOGY, 3, &found);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	res = searchMatchingOrder(escaper, BIOLOGY, 3, NULL);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

static bool testEscaperListOrders() {
	Escaper escaper = getExampleEscaper();
	EscaperResult res = escaperAddOrder(escaper, BIOLOGY, 2, 7, 8, 3);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 3, 9, 10, 4);
	List orders;
	res = escaperListOrders(escaper, &orders);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	Order order1 = listGetFirst(orders);
	Order order2 = listGetNext(orders);
	int room1, room2, faculty1, faculty2, hour1, hour2, day1, day2, ppl1, ppl2;
	ASSERT_TEST(orderGetRoomId(order1,&room1) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetRoomId(order2,&room2) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetRoomFaculty(order1,&faculty1) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetRoomFaculty(order2,&faculty2) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetDay(order1,&day1) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetDay(order2,&day2) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetHour(order1,&hour1) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetHour(order2,&hour2) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetNumPpl(order1,&ppl1) == ORDER_SUCCESS);
	ASSERT_TEST(orderGetNumPpl(order2,&ppl2) == ORDER_SUCCESS);
	ASSERT_TEST(room1 == 2 && faculty1 == BIOLOGY && day1 == 7
			    && hour1 == 8 && ppl1 == 3);
	ASSERT_TEST(room2 ==3 && faculty2 == COMPUTER_SCIENCE && day2 == 9
				&& hour2 == 10 && ppl2 == 4);
	listDestroy(orders);
	escaperDestroy(escaper);
	return true;
}

static bool testIsEscaperInRoom(){
	Escaper escaper = getExampleEscaper();
	EscaperResult res;
	bool in_room;
	res = isEscaperInRoom(escaper, 8, 7, &in_room);
	ASSERT_TEST(res == ESCAPER_SUCCESS && !in_room);
	res = escaperAddOrder(escaper, COMPUTER_SCIENCE, 2, 5, 8, 3);
	ASSERT_TEST(res == ESCAPER_SUCCESS);
	res = isEscaperInRoom(escaper, 8, 5, &in_room);
	ASSERT_TEST(res == ESCAPER_SUCCESS && in_room);
	res = isEscaperInRoom(NULL, 8, 5, &in_room);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	res = isEscaperInRoom(escaper, 8, 5, NULL);
	ASSERT_TEST(res == ESCAPER_NULL_PARAMETER);
	res = isEscaperInRoom(escaper, 24, 5, &in_room);
	ASSERT_TEST(res == ESCAPER_INVALID_PARAMETER);
	res = isEscaperInRoom(escaper, 1, -24, &in_room);
	ASSERT_TEST(res == ESCAPER_INVALID_PARAMETER);
	escaperDestroy(escaper);
	return true;
}

int main(){
	RUN_TEST(testEscaperCreate);
	RUN_TEST(testEscaperCopy);
	RUN_TEST(testEscaperGetEmail);
	RUN_TEST(testEscaperGetFaculty);
	RUN_TEST(testEscaperGetSkillLevel);
	RUN_TEST(testEscaperCompare);
	RUN_TEST(testEscaperNextDay);
	RUN_TEST(testEscaperAddOrder);
	RUN_TEST(testEscaperRemoveOldOrders);
	RUN_TEST(testEscaperMaximalRoomId);
	RUN_TEST(testSearchMatchingOrder);
	RUN_TEST(testEscaperListOrders);
	RUN_TEST(testIsEscaperInRoom);
	return 0;
}
