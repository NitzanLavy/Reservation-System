
#ifndef ROOM_INPUT_CHECKS_H_
#define ROOM_INPUT_CHECKS_H_

#include <stdbool.h>

#define MIN_HOUR 0
#define MAX_HOUR 24
#define MIN_DIFFICULTY 1
#define MAX_DIFFICULTY 10
#define DIVIDE_NUM 4
#define MIN_PRICE 1

/* Returns true if open_hour is between MIN_HOUR and MAX_HOUR-1 AND
 * close_hour is between MIN_HOUR+1 and MAX_HOUR AND close_hour is greater
 * than open_hour, false otherwise.
 */
bool isLegalOpeningHours(int open_hour, int close_hour);

/* Returns true if the given difficulty is between MIN_DIFFICULTY and
 * MAX_DIFFICULTY, false otherwise. */
bool isLegalDifficulty(int difficulty);


/* Returns true if the given price is divisible by DIVIDE_NUM and if its greater
 * than zero, false otherwise*/
bool isLegalPrice(int price);


#endif /* ROOM_INPUT_CHECKS_H_ */
