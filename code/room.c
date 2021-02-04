
#include "room.h"
#include "company.h"
#include <stdlib.h>
#include <stdio.h>
#include "room_input_checks.h"

struct room_s{
	int room_id;
	int price;
	int rec_num_ppl;
	int open_hour;
	int close_hour;
	int difficulty;
};


Room roomCreate(int room_id, int price, int rec_num_ppl,
				int open_hour, int close_hour, int difficulty,
				RoomResult *result){
	assert (result != NULL);
	Room new_room = malloc(sizeof(*new_room));
	if ( new_room == NULL ){
		*result = ROOM_OUT_OF_MEMORY;
		return NULL;
	}
	if ( !isLegalOpeningHours(open_hour, close_hour) || !isLegalPrice(price)
		 || !isLegalNumPpl(rec_num_ppl) || !isLegalDifficulty(difficulty)
		 || !isLegalId(room_id) ){
		*result = ROOM_INVALID_PARAMETER;
		free(new_room);
		return NULL;
	}
	new_room->room_id = room_id;
	new_room->price = price;
	new_room->rec_num_ppl = rec_num_ppl;
	new_room->open_hour = open_hour;
	new_room->close_hour = close_hour;
	new_room->difficulty = difficulty;
	*result = ROOM_SUCCESS;
	return new_room;
}


Room roomCopy(Room room, RoomResult *result){
	assert(result!=NULL);
	if ( room == NULL ){
		*result = ROOM_NULL_PARAMETER;
		return NULL;
	}
	Room new_room = NULL;
	new_room = roomCreate(room->room_id, room->price,
						  room->rec_num_ppl, room->open_hour, room->close_hour,
						  room->difficulty, result);
	return new_room;
}

int roomCompare(Room room1, Room room2){
	assert( room1 != NULL && room2 != NULL );
	return ((room1->room_id) - (room2->room_id));
}

void roomDestroy(Room room){
	if (room != NULL) {
		free(room);
	}
}

RoomResult getRoomId(Room room, int* id){
	if ( room == NULL || id == NULL ){
		return ROOM_NULL_PARAMETER;
	}
	*id = room->room_id;
	return ROOM_SUCCESS;
}

RoomResult getRecNumPpl(Room room, int* num_ppl){
	if ( room == NULL || num_ppl == NULL ){
		return ROOM_NULL_PARAMETER;
	}
	*num_ppl = room->rec_num_ppl;
	return ROOM_SUCCESS;
}

RoomResult getRoomDifficulty(Room room, int *difficulty){
	if ( room == NULL || difficulty == NULL ){
		return ROOM_NULL_PARAMETER;
	}
	*difficulty = room->difficulty;
	return ROOM_SUCCESS;
}

RoomResult getRoomPrice(Room room, int *price){
	if ( room == NULL || price == NULL ){
		return ROOM_NULL_PARAMETER;
	}
	*price = room->price;
	return ROOM_SUCCESS;
}


RoomResult getRoomOpenHours(Room room, int* open_hour, int* close_hour){
	if ( room == NULL || open_hour == NULL || close_hour == NULL ){
		return ROOM_NULL_PARAMETER;
	}
	*open_hour = room->open_hour;
	*close_hour = room->close_hour;
	return ROOM_SUCCESS;
}

