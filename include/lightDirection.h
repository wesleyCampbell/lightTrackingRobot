/*
 * @file lightDirection.h
 *
 * Contains methods for determining the direction of light using an array
 * of photodiodes.
 *
 * @author Wesley Campbell
 * @date 2026-03-30
 * @version v1.0.0
 */

#ifndef __lightDirection_h__
#define __lightDirection_h__

#include "includes.h"

#define PHOTODIODE_VOLTAGE_LIMIT 2.7

#define LIGHT_DOWN 0X0001
#define LIGHT_UP 0X0010
#define LIGHT_RIGHT 0X0100
#define LIGHT_LEFT 0X1000
typedef uint16_t LIGHT_DIR;

/**
 * Determines if light is detected on a photodiode pin
 *
 * @return true if light is detected, false otherwise
 */
bool isLight(int pin);

/**
 * Reads all photodiodes in the array and calculates the 
 * direction of the light
 *
 * @return A collection of direction flags
 */
LIGHT_DIR detectLightDirection();

#endif  // __lightDirection_h__
