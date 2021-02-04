
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "EscapeTechnion.h"
#include "mtm_ex3.h"
#include "parser.h"


/*
 * Receives argc and argv, and pointers to input and output channel.
 * The function checks if the first two input parameters are for input channel
 * and the last two parameters are for output channel, and initializes the
 * channel accordingly.
 */
static int firstInput (int argc,char** argv, FILE** input, FILE** output);

/*
 * Receives argc and argv, and pointers to input and output channel.
 * The function checks if the first two input parameters are for output channel
 * and the last two parameters are for input channel, and initializes the
 * channel accordingly.
 */
static int firstOutput (int argc,char** argv, FILE** input, FILE** output);

/*
 * Receives argc and argv, and pointers for input and output channels.
 * The function handles the case of only two input parameters - meaning only
 * for input or only for output channel. The function checks which channel is
 * relevant, and initializes the channels accordingly.
 */
static int oneInput (int argc,char** argv, FILE** input, FILE** output);

/*
 * Receives argc and argv, and pointers for input and output channels.
 * The function handles the case of four input parameters - meaning input
 * parameters for both input and output channels. The function checks which
 * channel's parameters are first, and initializes the channels accordingly.
 */
static int twoInputs (int argc, char** argv, FILE** input, FILE** output);

/*
 * Receives a number of slot in argv array.
 * The function returns true if num slot in argv array is -i, meaning relevant
 * for input channel, and false otherwise.
 */
static int isInputFile (int num, char** argv);

/*
 * Receives a number of slot in argv array.
 * The function returns true if num slot in argv array is -o, meaning relevant
 * for output channel, and false otherwise.
 */
static int isOutputFile (int num, char** argv);



static int firstInput (int argc, char** argv, FILE** input, FILE** output){
	*input = fopen(argv[2],"r");
	if (*input == NULL) {
		return 0;
	}
	if (isOutputFile(argc-2,argv)) {
		*output = fopen(argv[argc-1],"w");
		if (*output == NULL) {
			fclose (*input);
			return 0;
		}
		return 2;
	}
	return 1;
}


static int firstOutput (int argc,char** argv, FILE** input, FILE** output) {
	*output = fopen(argv[2],"w");
	if (*output == NULL) {
		return 0;
	}
	if (isInputFile(argc-2,argv)) {
		*input = fopen(argv[argc-1],"r");
		if (*input == NULL) {
			fclose (*output);
			return 0;
		}
		return 2;
	}
	return 1;
}


static int oneInput (int argc,char** argv, FILE** input, FILE** output) {
	if (isInputFile(argc-2,argv)) {
		*input = fopen(argv[argc-1],"r");
		if (*input == NULL) {
			return 0;
		}
		*output = stdout;
		return 2;
	}
	if (isOutputFile(argc-2,argv)) {
		*output = fopen(argv[argc-1],"w");
		if (*output == NULL) {
			return 0;
		}
		*input = stdin;
		return 2;
	}
	return 1;
}


static int twoInputs (int argc, char** argv, FILE** input, FILE** output) {
	if (isInputFile(1,argv)) {
		return firstInput (argc, argv, input, output);
	}
	if (isOutputFile(1,argv)) {
		return firstOutput (argc, argv, input, output);
	}
	return 1;
}


static int isInputFile (int num, char** argv) {
	if (strcmp (argv[num],"-i") == 0) {
		return true;
	}
	return false;
}


static int isOutputFile (int num, char** argv) {
	if (strcmp (argv[num],"-o") == 0) {
		return true;
	}
	return false;
}


int main (int argc, char** argv){
	if (argc != 1 && argc != 3 && argc != 5) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 1;
	}
	FILE *input, *output;
	int res = 2;

	if (argc == 1) {
		input = stdin;
		output = stdout;
	}
	if (argc == 3) {
		res = oneInput (argc, argv, &input, &output);
	}
	if (argc == 5) {
		res = twoInputs (argc, argv, &input, &output);
	}
	if (res == 0) {
		mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
		return 1;
	}
	if (res == 1) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 1;
	}
	EscapeTechnionResult et_result;
	EscapeTechnion escapeTechnion = addEscapeTechnion(&et_result);
	MtmErrorCode mtm_result = translateETMtm(et_result);
	if (mtm_result != MTM_SUCCESS) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 1;
	}
	int num = Parser(escapeTechnion, input, output);
	destroyEscapeTechnion(escapeTechnion);
	if (input != stdin) {
		fclose(input);
	}
	if (output != stdout) {
		fclose(output);
	}
	return num;
}




