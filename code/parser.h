
#ifndef PARSER_H_
#define PARSER_H_

/*
 * This function converts EscapeTechninoResult type messages to MtmErrorCode
 * messages.
 */
MtmErrorCode translateETMtm(EscapeTechnionResult et_result);

/*
 * The function receives input and output files. The function reads and
 * analyzes the commands and commands' parameters from the input file, and
 * calls to the relevant functions. The function uses output file as an input
 * parameter for some of the function it calls. Error messages are written to
 * stderr channel.
 * The function returns 1 if OUT_OF_MEMORY error message has been returned by
 * functions it calls to, 0 otherwise.
 */
int Parser (EscapeTechnion escapeTechnion, FILE* input, FILE* output);

#endif /* PARSER_H_ */
