/*
 * utils.h
 *
 *  Created on: Nov 19, 2024
 *      Author: LuisPC
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "stdint.h"
#include "stddef.h"

/* Constants for random number generation */
#define RNG_MULTIPLIER 1664525U   /* Multiplier for Linear Congruential Generator */
#define RNG_INCREMENT  1013904223U /* Increment for Linear Congruential Generator */
#define RNG_MODULUS    0xFFFFFFFFU /* Modulus for Linear Congruential Generator */

#define MIN_VAL_FOR_RAND	0 /* Minimum value allowed for the simulated temperature function */
#define MAX_VAL_FOR_RAND	40 /* Maximum value allowed for the simulated temperature function */

#define MAX_TEMP_INDEX      10 /* Maximum number of temperature log entries */

extern uint8_t temperature_log[MAX_TEMP_INDEX];

int get_temperature(void);
void add_new_temp_value(void);


#endif /* INC_UTILS_H_ */
