

#include <stdlib.h>
#include <stdio.h>

#include "room_input_checks.h"


bool isLegalOpeningHours(int open_hour, int close_hour){
	return ( ( MIN_HOUR <= open_hour && open_hour <= MAX_HOUR-1 )
			  && ( MIN_HOUR+1 <= close_hour && close_hour <= MAX_HOUR )
			  && ( open_hour < close_hour ) );
}

bool isLegalDifficulty(int difficulty){
	return ( MIN_DIFFICULTY <= difficulty && difficulty <= MAX_DIFFICULTY );
}

bool isLegalPrice(int price){
	return ( price >= MIN_PRICE && price%DIVIDE_NUM == 0 );
}
