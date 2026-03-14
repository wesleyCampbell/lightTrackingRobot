/**
 * @file capacitive_touch.cpp
 *
 * @brief Contains functions that deal with sensing capacitive touch.
 *
 * @author Wesley Campbell
 * @date 2026-03-13
 * @version 1.0.0
 */

#include "capacitive_touch.h"

long computeTau() {
	static CapacitiveSensor sensor = CapacitiveSensor(CAP_OUT_PIN, CAP_IN_PIN);
	long tau = sensor.capacitiveSensor(CAP_SENSOR_SAMPLES);
	
	return tau;
}

bool detectCapTouch() {
	long tau = computeTau();

#ifdef DEBUG_TAU
	Serial.print("tau: ");
	Serial.println(tau);
#endif

	if (tau > CAP_SENSOR_TAU_THRESHOLD) 
		return true;
	return false;
}
