/*
 * utils.c
 *
 *  Created on: Nov 19, 2024
 *      Author: LuisPC
 */

/**
  * @brief  Simulates retrieving the current temperature using random number generation
  * @retval Temperature value
  * @note	This function will always return a random value between 0 and 40 as those were the values chosen
  */

#include "utils.h"

uint8_t temperature_log[MAX_TEMP_INDEX] = {0};
uint8_t temp_index = 0;

static uint32_t seed = 12345U; /* Example seed value */

/**
  * @brief  Generates the next random number using Linear Congruential Generator (LCG)
  * @param  seed Pointer to the seed value
  * @retval Generated random number
  * @note   This function updates the seed and returns the next random value.
  */
static uint32_t generate_random(uint32_t *seed)
{
    /* Ensure the seed pointer is valid */
    if (seed == NULL)
    {
        return 0; /* Error case, return a default value */
    }

    /* Update the seed using the LCG formula */
    *seed = (RNG_MULTIPLIER * (*seed) + RNG_INCREMENT) & RNG_MODULUS;

    /* Return the generated random number */
    return *seed;
}

/**
  * @brief  Generates a random number in a specified range
  * @param  seed Pointer to the seed value
  * @param  min Minimum value of the range
  * @param  max Maximum value of the range
  * @retval Random number within the specified range
  * @note   This function scales the random value to the desired range [min, max].
  */
static uint32_t random_in_range(uint32_t *seed, uint32_t min, uint32_t max)
{
	/* Ensure valid input range */
    if (seed == NULL || min > max)
    {
        return 0; /* Error case, return a default value */
    }

    /* Generate a random number and scale it to the desired range */
    uint32_t rand_value = generate_random(seed);
    return min + (rand_value % (max - min + 1));
}

/**
  * @brief  Simulates retrieving the current temperature using random number generation
  * @retval Temperature value
  * @note	This function will always return a random value between 0 and 40 as those were the values chosen
  */
int get_temperature(void){

	uint32_t temp = random_in_range(&seed, MIN_VAL_FOR_RAND, MAX_VAL_FOR_RAND);
	return (int)temp; /* Ensure range is valid */

}

/**
  * @brief  Adds a new random temperature value to the circular buffer (temperature_log)
  */
void add_new_temp_value(void){

	if (temp_index < MAX_TEMP_INDEX) {

		/* Simulate logging a temperature value */
		temperature_log[temp_index] = get_temperature();

		temp_index++; /* Increment the log index */
	} else {
		temp_index = 0; /* Reset log index if maximum log size reached */
	}
}

/**
  * @brief  Returns a pointer to the temperature_log variable
  * @retval Pointer to temperature_log
  */
uint8_t* get_temperature_log() {
    return temperature_log;
}

