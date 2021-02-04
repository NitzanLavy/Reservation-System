
#ifndef ROOM_H_
#define ROOM_H_

#include <stdbool.h>
#include <stdlib.h>
#include "general_input_checks.h"

typedef struct room_s* Room;

typedef enum{
	ROOM_OUT_OF_MEMORY,
	ROOM_NULL_PARAMETER,
	ROOM_INVALID_PARAMETER,
	ROOM_SUCCESS
}RoomResult;

/* allocates a new room and sets it's fields' values according to the
 * given parameters. returns the new room if successful and NULL otherwise.
 * result is updated accordingly.*/
Room roomCreate(int room_id, int price, int rec_num_ppl,
				int open_hour, int close_hour, int difficulty,
				RoomResult *result);

/* returns a copy of the given room. returns NULL if it fails because of
 * memory problems or if the given room is NULL. updates result accordingly */
Room roomCopy(Room room, RoomResult *result);

/* returns a positive number if room1's id is greater than room2's id
 * returns 0 if room1's id is equal to room2's id
 * returns a negative number if room1's id is smaller than room2's id
 * assumes the given rooms do not point to NULL */
int roomCompare(Room room1, Room room2);

/* frees the allocation for room. does nothing if the given room is NULL */
void roomDestroy(Room room);

/* assigns the given rooms id to the given parameter
 * returns ROOM_NULL_PARAMETER if a parameter is NULL
 * returns ROOM_SUCCESS otherwise  */
RoomResult getRoomId(Room room, int* id);

/* returns the given room's recommended number of people. assumes room isn't
 * NULL */
RoomResult getRecNumPpl(Room room, int* num_ppl);

/* assigns the given room's difficulty to the given parameter
 * returns ROOM_NULL_PARAMETER if the given room is NULL
 * returns ROOM_SUCCESS otherwise */
RoomResult getRoomDifficulty(Room room, int* difficulty);

/* assigns the given room's price to the given price parameter
 * returns ROOM_NULL_PARAMETER if a given parameter is NULL
 * returns ROOM_SUCCESS otherwise */
RoomResult getRoomPrice(Room room, int *price);

/* assigns the given room's open hour and close hour to the given parameters.
 * returns ROOM_NULL_PARAMETER if a given parameter is NULL
 * return ROOM_SUCCESS otherwise */
 RoomResult getRoomOpenHours(Room room, int* open_hour, int* close_hour);
#endif /* ROOM_H_ */
