/*
 * @file lightDirection.cpp
 *
 * Contains methods for determining the direction of light using an array
 * of photodiodes.
 *
 * @author Wesley Campbell
 * @date 2026-03-30
 * @version v1.0.0
 */

#include "lightDirection.h"

bool isLight(int pin) {
	float voltage = readPinVoltage(pin);

	return voltage >= PHOTODIODE_VOLTAGE_LIMIT;
}

LIGHT_DIR detectLightDirection() {
	bool lightTopLeft = isLight(PHOTODIODE_TOP_LEFT);
	bool lightBotLeft = isLight(PHOTODIODE_BOTTOM_LEFT);
	bool lightBotRight = isLight(PHOTODIODE_BOTTOM_RIGHT);
	bool lightTopRight = isLight(PHOTODIODE_VOLTAGE_LIMIT);

	LIGHT_DIR dir = 0x0;

	// light detected up
	if (lightTopLeft && lightTopRight)
		dir |= LIGHT_UP;
	// light detected down
	if (lightBotLeft && lightBotRight)
		dir |= LIGHT_DOWN;
	// Light detected left
	if (lightTopLeft && lightBotLeft)
		dir |= LIGHT_LEFT;
	// Light detected right
	if (lightTopRight && lightBotRight)
		dir |= LIGHT_RIGHT;

	return dir;
}
