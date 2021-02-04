

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "EscapeTechnion.h"
#include "mtm_ex3.h"
#include "escape_technion_input_checks.h"
#include "parser.h"

#define MIN_DAY 0
#define ASCII_ZERO '0'
#define TOKEN " \n\t\r"

/*
 * Receives a char and translates it to an integer type digit.
 */
static int charToInt (char c);

/*
 * Receives a string. The function checks if the string is legal according to
 * the format "xx-yy", and returns in the parameters open_hour and close_hour
 * the relevant numbers.
 */
static bool isLegalWorkingHrs (char* working_hrs, int* open_hour,
		int* close_hour);

/*
 * The function receives two chars, converts them to integers and calculates
 * the hour number.
 */
static int convertHourToInt (char char1, char char2);

/*
 * Receives a time string. The function finds the '-' char, and calculates the
 * value of the number before and after the char. The calculated number is
 * assigned into day and hour accordingly.
 */
static bool isLegalTime(char* time,int* day,int* hour);

/*
 * This function is called when the first input string of a new line is
 * "company". The function checks the next string in the line and calls to the
 * relevant functions accordingly.
 */
static bool inputCompany (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "company", and the second input string of a new line is "add". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * addCompany function to add a new company.
 */
static bool inputCompanyAdd(EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "company", and the second input string of a new line is "remove". The
 * function assign all of the command's inputs to  parameters, and uses them to
 * call removeCompany function to add a new company.
 */
static bool inputCompanyRemove (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "room". The function checks the next string in the line and calls to the
 * relevant functions accordingly.
 */
static bool inputRoom (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "room", and the second input string of a new line is "add". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * addRoom function to add a new company.
 */
static bool inputRoomAdd (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "room", and the second input string of a new line is "remove". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * removeRoom function to add a new company.
 */
static bool inputRoomRemove (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "escaper". The function checks the next string in the line and calls to the
 * relevant functions accordingly.
 */
static bool inputEscaper (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "escaper", and the second input string of a new line is "add". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * addEscaper function to add a new company.
 */
static bool inputEscaprAdd (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "escaper", and the second input string of a new line is "remove". The
 * function assign all of the command's inputs to  parameters, and uses them to
 * call addEscaper function to add a new company.
 */
static bool inputEscaperRemove (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "escaper", and the second input string of a new line is "recommend". The
 * function assign all of the command's inputs to  parameters, and uses them to
 * call escaperRecommend function to add a new company.
 */
static bool inputEscaperRecommend (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "escaper", and the second input string of a new line is "order". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * addOrder function to add a new company.
 */
static bool inputEscaperOrder (EscapeTechnion escapeTechnion, char* tok);

/*
 * This function is called when the first input string of a new line is
 * "report". The function checks the next string in the line and calls to the
 * relevant functions accordingly.
 */
static bool inputReport(EscapeTechnion escapeTechnion, char* tok,FILE* output);

/*
 * This function is called when the first input string of a new line is
 * "report", and the second input string of a new line is "day". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * reportDay function to add a new company.
 */
static bool inputReportday (EscapeTechnion escapeTechnion, char* tok,
		FILE* output);

/*
 * This function is called when the first input string of a new line is
 * "report", and the second input string of a new line is "best". The function
 * assign all of the command's inputs to  parameters, and uses them to call
 * reportBestFaculties function to add a new company.
 */
static bool inputReportBest(EscapeTechnion escapeTechnion, char* tok,
							FILE* output);


MtmErrorCode translateETMtm(EscapeTechnionResult et_result) {
	switch (et_result) {
	case ET_OUT_OF_MEMORY:
		return MTM_OUT_OF_MEMORY;
	case ET_NULL_PARAMETER:
		return MTM_NULL_PARAMETER;
	case ET_INVALID_PARAMETER:
		return MTM_INVALID_PARAMETER;
	case ET_EMAIL_ALREADY_EXISTS:
		return MTM_EMAIL_ALREADY_EXISTS;
	case ET_COMPANY_EMAIL_DOES_NOT_EXIST:
		return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
	case ET_CLIENT_EMAIL_DOES_NOT_EXIST:
		return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
	case ET_ID_ALREADY_EXIST:
		return MTM_ID_ALREADY_EXIST;
	case ET_ID_DOES_NOT_EXIST:
		return MTM_ID_DOES_NOT_EXIST;
	case ET_CLIENT_IN_ROOM:
		return MTM_CLIENT_IN_ROOM;
	case ET_ROOM_NOT_AVAILABLE:
		return MTM_ROOM_NOT_AVAILABLE;
	case ET_RESERVATION_EXIST:
		return MTM_RESERVATION_EXISTS;
	case ET_NO_ROOMS_AVAILABLE:
		return MTM_NO_ROOMS_AVAILABLE;
	case ET_SUCCESS:
		return MTM_SUCCESS;
	default:
		return -1;
	}
}


int Parser (EscapeTechnion escapeTechnion, FILE* input, FILE* output) {
	char line[MAX_LEN]="";
	while (fgets(line, MAX_LEN, input) != NULL) {
		char* tok = strtok (line,TOKEN);
		while (tok != NULL && tok[0] != '#') {
			if (strcmp (tok,"company") == 0) {
				if (inputCompany(escapeTechnion, tok) == 0) {
					return 1;
				}
				break;
			}
			if (strcmp (tok,"room") == 0) {
				if (inputRoom(escapeTechnion,tok) == 0) {
					return 1;
				}
				break;
			}
			if (strcmp (tok,"escaper") == 0) {
				if (inputEscaper(escapeTechnion, tok) == 0) {
					return 1;
				}
				break;
			}
			if (strcmp (tok,"report") == 0) {
				if (inputReport(escapeTechnion, tok, output) == 0) {
					return 1;
				}
				break;
			}
			mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
			break;
		}
	}
	return 0;
}


static bool inputCompany (EscapeTechnion escapeTechnion, char* tok) {
	tok = strtok (NULL,TOKEN);
	if (tok == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	if (strcmp(tok,"add") == 0) {
		return inputCompanyAdd(escapeTechnion,tok);
	}
	if (strcmp(tok,"remove") == 0) {
		return inputCompanyRemove(escapeTechnion, tok);
	}
	return 1;
}

static bool inputCompanyAdd (EscapeTechnion escapeTechnion, char* tok) {
	char* email = strtok (NULL,TOKEN);
	int faculty = atoi(strtok (NULL,TOKEN));
	if (!isLegalEmail(email) || !isLegalFaculty(faculty)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = addCompany(escapeTechnion, email, faculty);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputCompanyRemove (EscapeTechnion escapeTechnion, char* tok) {
	char* email = strtok (NULL,TOKEN);
	if (!isLegalEmail(email)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = removeCompany(escapeTechnion, email);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputRoom (EscapeTechnion escapeTechnion, char* tok) {
	tok = strtok (NULL,TOKEN);
	if (tok == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	if (strcmp(tok,"add") == 0) {
		return inputRoomAdd(escapeTechnion,tok);
	}
	if (strcmp(tok,"remove") == 0) {
		return inputRoomRemove(escapeTechnion, tok);
	}
	return 1;
}

static bool inputRoomAdd (EscapeTechnion escapeTechnion, char* tok){
	char* email = strtok (NULL,TOKEN);
	int id = atoi(strtok (NULL,TOKEN));
	int price = atoi(strtok (NULL,TOKEN));
	int num_ppl = atoi(strtok (NULL,TOKEN));
	char* working_hrs = strtok (NULL,TOKEN);
	int difficulty = atoi(strtok (NULL,TOKEN));
	int open_hour, close_hour;
	if (!isLegalEmail(email) || !isLegalId(id) || !isLegalPrice(price) ||
			!isLegalNumPpl(num_ppl) ||!isLegalWorkingHrs(working_hrs,
			&open_hour,&close_hour) ||!isLegalDifficulty(difficulty)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = addRoom(escapeTechnion, email, id, price,
			num_ppl, open_hour, close_hour, difficulty);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputRoomRemove(EscapeTechnion escapeTechnion, char* tok){
	int faculty = atoi(strtok (NULL,TOKEN));
	int id = atoi(strtok (NULL,TOKEN));
	if (!isLegalFaculty(faculty) || !isLegalId(id)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = removeRoom(escapeTechnion, faculty, id);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputEscaper (EscapeTechnion escapeTechnion, char* tok) {
	tok = strtok (NULL,TOKEN);
	if (tok == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	if (strcmp(tok,"add") == 0) {
		return inputEscaprAdd(escapeTechnion,tok);
	}
	if (strcmp(tok,"remove") == 0) {
		return inputEscaperRemove(escapeTechnion,tok);
	}
	if (strcmp(tok,"order") == 0) {
		return inputEscaperOrder(escapeTechnion,tok);
	}
	if (strcmp(tok,"recommend") == 0) {
		return inputEscaperRecommend(escapeTechnion,tok);
	}
	return 1;
}

static bool inputEscaprAdd (EscapeTechnion escapeTechnion, char* tok) {
	char* email = strtok (NULL,TOKEN);
	int faculty = atoi(strtok (NULL,TOKEN));
	int skill_level = atoi(strtok (NULL,TOKEN));
	if (!isLegalEmail(email) || !isLegalFaculty(faculty) ||
			!isLegalSkillLevel(skill_level)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = addEscaper(escapeTechnion, email, faculty,
			skill_level);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputEscaperRemove (EscapeTechnion escapeTechnion, char* tok) {
	char* email = strtok (NULL,TOKEN);
	if (!isLegalEmail(email)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = removeEscaper(escapeTechnion, email);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputEscaperOrder (EscapeTechnion escapeTechnion, char* tok) {
	char* email = strtok (NULL,TOKEN);
	int faculty = atoi(strtok (NULL,TOKEN));
	int id = atoi(strtok (NULL,TOKEN));
	char* time = strtok (NULL,TOKEN);
	int num_ppl = atoi(strtok (NULL,TOKEN));
	int day, hour;
	if (!isLegalEmail(email) || !isLegalFaculty(faculty) || !isLegalId(id) ||
			!isLegalTime(time,&day,&hour) || !isLegalNumPpl(num_ppl)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = addOrder (escapeTechnion, email, faculty,
			id, day, hour, num_ppl);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputEscaperRecommend (EscapeTechnion escapeTechnion, char* tok) {
	char* email = strtok (NULL,TOKEN);
	int num_ppl = atoi(strtok (NULL," \n"));
	if (!isLegalEmail(email) ||!isLegalNumPpl(num_ppl)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	EscapeTechnionResult et_result = escaperRecommended (escapeTechnion,
			email, num_ppl);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputReport (EscapeTechnion escapeTechnion, char* tok,
		FILE* output) {
	tok = strtok (NULL,TOKEN);
	if (tok == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETER);
		return 1;
	}
	if (strcmp(tok,"day") == 0) {
		return inputReportday(escapeTechnion,tok,output);
	}
	if (strcmp(tok,"best") == 0) {
		return inputReportBest(escapeTechnion,tok,output); // added output
	}
	return 1;
}

static bool inputReportday (EscapeTechnion escapeTechnion, char* tok,
		FILE* output) {
	EscapeTechnionResult et_result = reportDay(escapeTechnion,output);
	MtmErrorCode mtm_res = translateETMtm(et_result);
	if (mtm_res != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, mtm_res);
		if (mtm_res == MTM_OUT_OF_MEMORY) {
			return 0;
		}
	}
	return 1;
}

static bool inputReportBest (EscapeTechnion escapeTechnion, char* tok,
							 FILE* output) { // added output
	reportBestFaculties(escapeTechnion, output);
	return 1;
}


static bool isLegalWorkingHrs (char* working_hrs, int* open_hour,
		int* close_hour) {
	if (strlen(working_hrs) != 5) {
		return false;
	}
	if (working_hrs[2] != '-') {
		return false;
	}
	*open_hour = convertHourToInt(working_hrs[0], working_hrs[1]);
	*close_hour = convertHourToInt(working_hrs[3], working_hrs[4]);

	if (*close_hour <= *open_hour) {
		return false;
	}
	return isLegalOpeningHours (*open_hour,*close_hour);
}

static int convertHourToInt (char char1, char char2) {
	int num1 = charToInt(char1);
	int num2 = charToInt(char2);
	return (num1*10)+num2;
}

static int charToInt (char c) {
	return c-ASCII_ZERO;
}

static bool isLegalTime(char* time,int* day,int* hour) {
	int i=0, place=0;
	while (time[i]!='\0' && time[i]!='-') {
		if (time[i+1] == '-') {
			place = i+1;
		}
		i++;
	}
	if (time[i]=='\0') {
		return false;
	}
	*day = charToInt(time[0]);
	*hour = charToInt(time[place+1]);
	if (*day != 0) {
		for (int i=1;i<place;i++) { // changed to i = 1
			*day = (*day *10) + charToInt(time[i]);
		}
	}
	i=i+2;
	for (;i<strlen(time);i++) {
		*hour = (*hour *10) + charToInt(time[i]);
	}
	if (*day < MIN_DAY || *hour<MIN_HOUR || *hour > MAX_HOUR) {
		return false;
	}
	return true;
}

