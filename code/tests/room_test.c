

#include "test_utilities.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../room.h"

static Room getExampleRoom() {
	RoomResult res;
	Room room = roomCreate(1, 16, 2, 20, 22, 5, &res);
	ASSERT_TEST(res==ROOM_SUCCESS);
	return room;
}

static bool testRoomCreate() {
	RoomResult res;
	Room room;
	room = roomCreate(1, 16, 2, 20, 22, 5, &res);
	ASSERT_TEST(res == ROOM_SUCCESS); // all parameters are legal
	roomDestroy(room);
	room = roomCreate(-1, 16, 2, 20, 22, 5, &res);  // id is negative
	ASSERT_TEST( (room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 5, 2, 20, 22, 5, &res); // illegal price (5)
	ASSERT_TEST( (room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 16, 0, 20, 22, 5, &res);
	// recommended number of people isn't a positive number
	ASSERT_TEST( ( room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 16, 2, 25, 26, 5, &res); // open hour > 23
	ASSERT_TEST( ( room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 16, 2, 20, 19, 5, &res);
	// open hour > close hour
	ASSERT_TEST( (room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 16, 2, 20, 26, 5, &res); // close hour > 24
	ASSERT_TEST( (room==NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 16, 2, 20, 22, 0, &res);
	// difficulty is below one
	ASSERT_TEST( (room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	room = roomCreate(1, 16, 2, 20, 22, 11, &res);
	// difficulty is over 10
	ASSERT_TEST( (room == NULL) && (res == ROOM_INVALID_PARAMETER) );
	roomDestroy(room);
	return true;
}

static bool testRoomCopy() {
	Room room1 = getExampleRoom();
	RoomResult res;
	Room room2 = roomCopy(room1, &res);
	// all parameters are legal
	ASSERT_TEST( room2!=NULL && roomCompare(room1, room2) == 0 &&
				 res == ROOM_SUCCESS );
	roomDestroy(room2);
	room2 = roomCopy(NULL, &res);
	// NULL parameter
	ASSERT_TEST( room2==NULL && res == ROOM_NULL_PARAMETER );
	roomDestroy(room2);
	roomDestroy(room1);
	return true;
}

static bool testGetRoomId() {
	RoomResult res;
	Room room = roomCreate(9, 16, 2, 22, 23, 9, &res);
	// create a room with an id that equals 9
	ASSERT_TEST(res == ROOM_SUCCESS);
	int room_id;
	res = getRoomId(room, &room_id);
	ASSERT_TEST(res == ROOM_SUCCESS && room_id == 9);
	roomDestroy(room);
	return true;
}

static bool testRoomCompare() {
	Room room1 = getExampleRoom();
	Room room2 = getExampleRoom();
	// both of the rooms will have the same id.
	int result = roomCompare(room1, room2);
	ASSERT_TEST( result == 0 );
	roomDestroy(room2);
	int id;
	RoomResult room_res = getRoomId(room1, &id);
	ASSERT_TEST(room_res == ROOM_SUCCESS);
	RoomResult create_res;
	room2 = roomCreate(id+1, 16, 2, 22, 23, 9, &create_res);
	// room2's id is greater than room1's id.
	ASSERT_TEST( create_res == ROOM_SUCCESS );
	result = roomCompare(room1, room2);
	ASSERT_TEST( result < 0 );
	result = roomCompare(room2, room1);
	ASSERT_TEST( result > 0);
	roomDestroy(room2);
	roomDestroy(room1);
	return true;
}

static bool testGetRecNumPpl() {
	RoomResult res;
	Room room = roomCreate(9, 16, 2, 22, 23, 9, &res);
	// create a room with an id that equals 2
	ASSERT_TEST(res == ROOM_SUCCESS);
	int num;
	res = getRecNumPpl(room, &num);
	ASSERT_TEST(res == ROOM_SUCCESS && num == 2);
	res = getRecNumPpl(NULL, &num);
	ASSERT_TEST(res == ROOM_NULL_PARAMETER);
	roomDestroy(room);
	return true;
}

static bool testGetRoomDifficulty() {
	RoomResult res;
	Room room = roomCreate(9, 16, 2, 22, 23, 8, &res);
	// create a room with difficulty that equals 8
	ASSERT_TEST(res == ROOM_SUCCESS);
	int diff;
	res = getRoomDifficulty(room, &diff);
	ASSERT_TEST(res == ROOM_SUCCESS && diff == 8);
	res = getRoomDifficulty(NULL, &diff);
	ASSERT_TEST(res == ROOM_NULL_PARAMETER);
	roomDestroy(room);
	return true;
}

static bool testGetRoomPrice() {
	RoomResult res;
	Room room = roomCreate(9, 16, 2, 22, 23, 8, &res);
	// create a room with a price that equals 16
	ASSERT_TEST(res == ROOM_SUCCESS);
	int price;
	res = getRoomPrice(room, &price);
	ASSERT_TEST(res == ROOM_SUCCESS && price == 16);
	res = getRoomPrice(NULL, &price);
	ASSERT_TEST(res == ROOM_NULL_PARAMETER);
	roomDestroy(room);
	return true;
}
/* RoomResult getRoomOpenHours(Room room, int* open_hour, int* close_hour);*/
static bool testGetRoomOpenHours(){
	RoomResult res;
	Room room = roomCreate(9, 16, 2, 22, 23, 8, &res);
	// create a room with open hour 22 and close hour 23
	ASSERT_TEST(res == ROOM_SUCCESS);
	int open, close;
	res = getRoomOpenHours(room, &open, &close);
	ASSERT_TEST(res == ROOM_SUCCESS && open == 22 && close == 23);
	res = getRoomOpenHours(NULL, &open, &close);
	ASSERT_TEST(res == ROOM_NULL_PARAMETER);
	roomDestroy(room);
	return true;
}

int main(){
	RUN_TEST(testRoomCreate);
	RUN_TEST(testRoomCompare);
	RUN_TEST(testRoomCopy);
	RUN_TEST(testGetRoomId);
	RUN_TEST(testGetRecNumPpl);
	RUN_TEST(testGetRoomDifficulty);
	RUN_TEST(testGetRoomPrice);
	RUN_TEST(testGetRoomOpenHours);
	return 0;
}


