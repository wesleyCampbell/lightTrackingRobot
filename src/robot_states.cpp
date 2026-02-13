/**
 * @file 	robot_states.c
 * @brief 	Implemenation of robot state functionality
 *
 * Implements robot state logic outlined in robot_states.h.
 * For documentation, see above mentioned header file.
 *
 * This file is part of the lightTrackingRobotProject for the BYU ECEN240 course.
 *
 * @author 	Wesley Campbell
 * @date 	2026-01-15
 * @version	v1.0.1
 */

#include "includes.h"
#include "params.h"
#include "robot_states.h"

detectionDataStruct detectedData;

actionStateStruct actionStates;

// ========================== DETECTION STATE FUNCTIONS =============================

float readPinVoltage(uint8_t pin) {
	// Maps the reading of the analog pin to the voltage scale
	return VOLTAGE_MAX * (float) analogRead(pin) / SENSOR_MAX_OUT;
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

	// Left light detection
	if (buttonPushed(BUTTON_MOTOR_LEFT)) {
		detectedData.lightDetected.left = DETECTION_TRUE;
	} else {
		detectedData.lightDetected.left = DETECTION_FALSE;
	}
	
	// Right light detection
	if (buttonPushed(BUTTON_MOTOR_RIGHT)) {
		detectedData.lightDetected.right = DETECTION_TRUE;
	} else {
		detectedData.lightDetected.right = DETECTION_FALSE;
	}


	// Light down detection
	if (buttonPushed(BUTTON_SERVO_DOWN)) {
		detectedData.lightDetected.down = DETECTION_TRUE;
	} else {
		detectedData.lightDetected.down = DETECTION_FALSE;
	}
	
	// Light up detection
	if (buttonPushed(BUTTON_SERVO_UP)) {
		detectedData.lightDetected.up = DETECTION_TRUE;
	} else {
		detectedData.lightDetected.up = DETECTION_FALSE;
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
	}

}

void fsmTempLightDetection() {
	// Left light planning
	switch(detectedData.lightDetected.left) {
		case DETECTION_FALSE:
			actionStates.Drive &= ~DRIVE_LEFT; 
			break;
		case DETECTION_TRUE:
			actionStates.Drive |= DRIVE_LEFT;
			break;
	}
	// Right light planning
	switch(detectedData.lightDetected.right) {
		case DETECTION_FALSE:
			actionStates.Drive &= ~DRIVE_RIGHT;
			break;
		case DETECTION_TRUE:
			actionStates.Drive |= DRIVE_RIGHT;
			break;
	}
}

void fsmServoMovement() {
	// Down light planning
	switch(detectedData.lightDetected.down) {
		case DETECTION_FALSE:
			actionStates.Servo &= ~SERVO_MOVE_DOWN;
			break;
		case DETECTION_TRUE:
			actionStates.Servo |= SERVO_MOVE_DOWN;
			break;
	}
	// Up light planning
	switch(detectedData.lightDetected.up) {
		case DETECTION_FALSE:
			actionStates.Servo &= ~SERVO_MOVE_UP;
			break;
		case DETECTION_TRUE:
			actionStates.Servo |= SERVO_MOVE_UP;
			break;
	}
	
	// If there is both light and above, stop the servo
	if ((actionStates.Servo & SERVO_MOVE_UP_DOWN) == SERVO_MOVE_UP_DOWN) {
		actionStates.Servo = SERVO_MOVE_STOP;
	}
}

void RobotPlanning() {
	fsmCollisionDetection();
	fsmTempLightDetection();
	fsmServoMovement();
}

// ================================ ACTION STATE FUNCTIONS ======================================

void RobotAction() {
	uint8_t status;
    handleCollisionAction();

	handleDriveAction();

	handleServoAction();

	printRobotState(&detectedData, &actionStates);

	sendPinData(BUTTON_SERVO_DOWN, readPinVoltage(BUTTON_SERVO_DOWN));
	sendPinData(BUTTON_SERVO_UP, readPinVoltage(BUTTON_SERVO_UP));
	// sendPinData(BUTTON_MOTOR_LEFT, readPinVoltage(BUTTON_MOTOR_LEFT));
	// sendPinData(BUTTON_MOTOR_RIGHT, readPinVoltage(BUTTON_MOTOR_RIGHT));
	sendPinData(BUTTON_COLLISION, readPinVoltage(BUTTON_COLLISION));
}	

void handleDriveAction() {
	// If DRIVE_LEFT flag set, drive left
	if (actionStates.Drive & DRIVE_LEFT) {
		activateLED(LED_MOTOR_LEFT);
	} else {
		disableLED(LED_MOTOR_LEFT);
	}
	// If DRIVE_RIGHT flag set, drive right
	if (actionStates.Drive & DRIVE_RIGHT) {
		activateLED(LED_MOTOR_RIGHT);
	} else {
		disableLED(LED_MOTOR_RIGHT);
	}
}

void handleCollisionAction() {
	switch(actionStates.Collision) {
		// If there is no collition, do nothing
		case COLLISION_INACTIVE:
			disableLED(LED_COLLISION);
			break;
		case COLLISION_ACTIVE:
			activateLED(LED_COLLISION);
			// stop the robot
			actionStates.Drive = DRIVE_STOP;
			break;
	}
}

void handleServoAction() {
	// If SERVO_MOVE_DOWN flag set, move servo down
	if (actionStates.Servo & SERVO_MOVE_DOWN) {
		activateLED(LED_SERVO_DOWN);
	} else {
		disableLED(LED_SERVO_DOWN);
	}
	// If SERVO_MOVE_UP flag set, move servo up
	if (actionStates.Servo & SERVO_MOVE_UP) {
		activateLED(LED_SERVO_UP);
	} else {
		disableLED(LED_SERVO_UP);
	}
}

void activateLED(uint8_t ledPin) {
	digitalWrite(ledPin, HIGH);
}

void disableLED(uint8_t ledPin) {
	digitalWrite(ledPin, LOW);
}
