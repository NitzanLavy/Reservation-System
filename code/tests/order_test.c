

#include "test_utilities.h"
#include "../order.h"
#include "../mtm_ex3.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


static bool testOrderCreate() {
	OrderResult res;
	Order order;
	order = orderCreate (2, 3, 4, 5, 6, &res);
	ASSERT_TEST(order != NULL && res == ORDER_SUCCESS);
	orderDestroy(order);
	order = orderCreate (1, 4, 0, 6, 4, &res);
	ASSERT_TEST(order != NULL && res == ORDER_SUCCESS);
	orderDestroy(order);
	order = orderCreate (5, 2, -3, 3, 7, &res);
	ASSERT_TEST(order == NULL && res == ORDER_INVALID_PARAMETER);
	orderDestroy(order);
	order = orderCreate (3, 8, 5, 24, 6, &res);
	ASSERT_TEST(order == NULL && res == ORDER_INVALID_PARAMETER);
	orderDestroy(order);
	order = orderCreate (7, 2, 7, -1, 1, &res);
	ASSERT_TEST(order == NULL && res == ORDER_INVALID_PARAMETER);
	orderDestroy(order);
	order = orderCreate (7, 2, 2, 5, 0, &res);
	ASSERT_TEST(order == NULL && res == ORDER_INVALID_PARAMETER);
	orderDestroy(order);
	order = orderCreate (7, 2, 2, 5, -3, &res);
	ASSERT_TEST(order == NULL && res == ORDER_INVALID_PARAMETER);
	orderDestroy(order);
	return true;
}

static bool testOrderCopy() {
	OrderResult res;
	Order order1 = orderCreate (2,5,3,1,6,&res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	Order order2 = orderCopy (order1, &res);
	int num1, num2, num3, num4, num5, num6;
	res = orderGetHour(order2, &num1);
	ASSERT_TEST(res == ORDER_SUCCESS && num1 == 1);
	ASSERT_TEST(order2 != NULL && res == ORDER_SUCCESS &&
			orderGetDay(order1, &num1) == orderGetDay(order2, &num2) &&
			orderGetHour(order1, &num3) == orderGetHour(order2, &num4) &&
			orderGetNumPpl(order1, &num5) == orderGetNumPpl(order2, &num6) &&
			num1 == num2 && num3 == num4 && num5 == num6);
	orderDestroy (order2);
	Order order3 = orderCopy (NULL, &res);
	ASSERT_TEST(order3 == NULL && res == ORDER_NULL_PARAMETER);
	orderDestroy(order3);
	orderDestroy(order1);
	return true;
}

static bool testOrderDecDay() {
	OrderResult res;
	Order order = orderCreate (6,2,4,3,2,&res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	res = orderDecDay (order);
	int num;
	res = orderGetDay(order, &num);
	ASSERT_TEST(res == ORDER_SUCCESS && num == 3);

	res = orderDecDay (NULL);
	ASSERT_TEST(res == ORDER_NULL_PARAMETER);
	orderDestroy(order);
	return true;
}

static bool testOrderGetDay() {
	OrderResult res;
	Order order = orderCreate (3,5,1,6,10,&res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	int num;
	res = orderGetDay (order, &num);
	ASSERT_TEST(res == ORDER_SUCCESS && num == 1);
	res = orderGetDay (NULL, &num);
	ASSERT_TEST(res == ORDER_NULL_PARAMETER);
	orderDestroy(order);
	return true;
}

static bool testOrderGetHour() {
	OrderResult res;
	Order order = orderCreate (5,1,6,8,2,&res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	int num;
	res = orderGetHour (order, &num);
	ASSERT_TEST(res == ORDER_SUCCESS && num == 8);
	res = orderGetHour (NULL, &num);
	ASSERT_TEST(res == ORDER_NULL_PARAMETER);
	orderDestroy(order);
	return true;
}

static bool testOrderGetNumPpl() {
	OrderResult res;
	Order order = orderCreate (1,2,3,4,5,&res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	int num;
	res = orderGetNumPpl (order, &num);
	ASSERT_TEST(res == ORDER_SUCCESS && num == 5);
	res = orderGetNumPpl (NULL, &num);
	ASSERT_TEST(res == ORDER_NULL_PARAMETER);
	orderDestroy(order);
	return true;
}

static bool testOrderGetRoomId(){
	OrderResult res;
	Order order = orderCreate(1, 2, 3, 4, 5, &res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	int id;
	res = orderGetRoomId(order, &id);
	ASSERT_TEST(res == ORDER_SUCCESS && id == 2);
	res = orderGetRoomId(NULL, &id);
	ASSERT_TEST(res == ORDER_NULL_PARAMETER);
	orderDestroy(order);
	return true;
}

static bool testOrderGetRoomFaculty(){
	OrderResult res;
	Order order = orderCreate(1, 2, 3, 4, 5, &res);
	ASSERT_TEST(res == ORDER_SUCCESS);
	int faculty;
	res = orderGetRoomFaculty(order, &faculty);
	ASSERT_TEST(res == ORDER_SUCCESS && faculty == 1);
	res = orderGetRoomFaculty(NULL, &faculty);
	ASSERT_TEST(res == ORDER_NULL_PARAMETER);
	orderDestroy(order);
	return true;
}

int main(){
	RUN_TEST(testOrderCreate)
	RUN_TEST(testOrderCopy);
	RUN_TEST(testOrderDecDay);
	RUN_TEST(testOrderGetDay);
	RUN_TEST(testOrderGetHour);
	RUN_TEST(testOrderGetNumPpl);
	RUN_TEST(testOrderGetRoomId);
	RUN_TEST(testOrderGetRoomFaculty);
	return 0;
}

