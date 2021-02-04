

#include <stdlib.h>
#include <stdio.h>
#include "order_input_checks.h"


bool isLegalDay(int day){
	return ( day > MIN_DAY -1 );
}

bool isLegalOrderHour(int hour){
	return ( MIN_ORDER_HOUR <= hour && hour <= MAX_ORDER_HOUR );
}
