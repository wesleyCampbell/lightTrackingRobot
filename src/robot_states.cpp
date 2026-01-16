/**
 * @file 	robot_states.c
 * @brief 	Implemenation of robot state functionality
 *
 * Implements robot state logic outlined in robot_states.h.
 * For documentation, see above mentioned header file.
 *
 * @author 	Wesley Campbell
 * @date 	2026-01-15
 * @version	v1.0.0
 */

#include "includes.h"
#include "robot_states.h"

detectionDataStruct detectedData;

actionStateStruct actionStates;

// ========================== DETECTION STATE FUNCTIONS =============================

float readPinVoltage(uint8_t pin) {
	// return VOLTAGE_MAX * (float) analogRead(pin) / SENSOR_MAX_OUT;
	return pin * 3.141;
}

bool buttonPushed(uint8_t button_pin) {
	if (analogRead(button_pin) >= BUTTON_VTHRESHOLD)
		return true;
	else
		return false;
}

bool collisionDetected() {
	// Currently, a button press will simulate a collision
	if (buttonPushed(BUTTON_COLLISION))
		return true;
	else
		return false;
}

void RobotDetection() {
	// Check for an immemant collision
	if (collisionDetected()) {
		detectedData.collisionDetected = DETECTION_TRUE;
	} else {
		detectedData.collisionDetected = DETECTION_FALSE;
	}
}

// =========================== PLANNING STATE FUNCTIONS ===============================

void fsmCollisionDetection() {
	switch(detectedData.collisionDetected) {
		// No collision detected
		case DETECTION_FALSE:
			actionStates.Collision = COLLISION_INACTIVE; // set all clear flag
			break;
		case DETECTION_TRUE:
			actionStates.Collision = COLLISION_ACTIVE; // set collision flag
			break;
		default:
			break;
	}
}

void RobotPlanning() {
	fsmCollisionDetection();
}

// ================================ ACTION STATE FUNCTIONS ======================================

void RobotAction() {
	handleCollisionAction();
}	

void handleCollisionAction() {
	switch(actionStates.Collision) {
		case COLLISION_INACTIVE:
			disableLED(LED_COLLISION);
			break;
		case COLLISION_ACTIVE:
			activateLED(LED_COLLISION);
			break;
	}
}

void activateLED(uint8_t ledPin) {
	digitalWrite(ledPin, HIGH);
}

void disableLED(uint8_t ledPin) {
	// TODO: Implemnt this
	digitalWrite(ledPin, LOW);
}
