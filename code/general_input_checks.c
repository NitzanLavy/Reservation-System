

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "general_input_checks.h"
#include "mtm_ex3.h"


bool isLegalEmail(char* email){
	assert (email!=NULL);
	int count = 0;
	while ( *email ){
		if ( *email == EMAIL_CHAR ){
			count++;
		}
		email++;
	}
	return ( count == 1 );
}

bool isLegalFaculty(int faculty){
	return ( 0 <= faculty && faculty < UNKNOWN );
}

bool isLegalId(int id){
	return (id > MIN_ID );
}

bool isLegalNumPpl(int num_rec_ppl){
	return ( num_rec_ppl >= MIN_NUM_PPL );
}

bool isLegalSkillLevel(int skill_level){
	return ( MIN_LEVEL <= skill_level && skill_level <= MAX_LEVEL );
}
