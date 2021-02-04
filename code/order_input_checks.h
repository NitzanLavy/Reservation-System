
#ifndef ORDER_INPUT_CHECKS_H_
#define ORDER_INPUT_CHECKS_H_

#include <stdbool.h>

#include "general_input_checks.h"

#define MIN_DAY 0
#define MIN_ORDER_HOUR 0
#define MAX_ORDER_HOUR 23
#define MIN_NUM_PPL_ORDER 1


/* returns true if the given day greater than MIN_DAY-1 and false otherwise */
bool isLegalDay(int day);

/* returns true if the given hour is between MIN_ORDER_HOUR and MAX_ORDER_HOUR
 * and false otherwise
 */
bool isLegalOrderHour(int hour);

#endif /* ORDER_INPUT_CHECKS_H_ */
