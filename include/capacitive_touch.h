/**
 * @file capacitive_touch.h
 *
 * @brief Contains functions that deal with sensing capacitive touch.
 *
 * @author Wesley Campbell
 * @date 2026-03-13
 * @version 1.0.0
 */

#ifndef __CAPACITIVE_TOUCH_H__
#define __CAPACITIVE_TOUCH_H__

#include "includes.h"
#include <CapacitiveSensor.h>

#define CAP_OUT_PIN         7 
#define CAP_IN_PIN  9 
#define CAP_SENSOR_SAMPLES 40
#define CAP_SENSOR_TAU_THRESHOLD 300 

enum CAP_STATE {CAP_WAITING, CAP_PRESSED, CAP_RELEASED};

/**
 * This function will compute the tau transient value of the capacitive sensor.
 *
 * @return tau
 */
long computeTau();

/**
 * This function will determine if there is currently a capacitive touch on the sensor.
 *
 * @return true if touch detected, false otherwise
 */
bool detectCapTouch();


#endif
