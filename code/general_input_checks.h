

#ifndef GENERAL_INPUT_CHECKS_H_
#define GENERAL_INPUT_CHECKS_H_

#include <stdbool.h>

#define MIN_ID 0
#define EMAIL_CHAR '@'
#define MIN_NUM_PPL 1
#define MAX_LEVEL 10
#define MIN_LEVEL 1


/* returns true if the given email is legal - if it has exactly one
 * EMAIL_CHAR. returns false otherwise */
bool isLegalEmail(char* email);

/* returns true if the faculty is between 0 (including) and UNKNOWN */
bool isLegalFaculty(int faculty);

/* returns true if the given id is greater than MIN_ID-1 and false otherwise */
bool isLegalId(int id);

/* Returns true if the given recommended number of people is greater than
 * or equal to MIN_NUM_REC_PPL, false otherwise.
 */
bool isLegalNumPpl(int num_ppl);

/*
 * Receives a number and returns true if the input number is between the
 * defined values MIN_SKILL_LEVEL and MAX_SKILL_LEVEL, false otherwise.
 */
bool isLegalSkillLevel(int skill_level);

#endif /* GENERAL_INPUT_CHECKS_H_ */
