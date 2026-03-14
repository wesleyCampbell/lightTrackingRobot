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
 * @version	v1.0.4
 */

#include "capacitive_touch.h"
#include "includes.h"
#include "params.h"
#include "robot_states.h"

detectionDataStruct detectedData;

actionStateStruct actionStates;

float batteryVoltage = 0;
BATTERY_LEVEL batteryVoltageLevel;

int capacitiveTouchDetected = DETECTION_FALSE;
CAP_STATE capState = CAP_WAITING;
ROBOT_SPEED robotSpeed = SLOW;

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

void readBatteryVoltage() {
	uint16_t voltageReading =  analogRead(BATTERY_PIN); 

	Serial.println(voltageReading);

	batteryVoltage = BATTERY_MAX_VOLTAGE * (float) voltageReading / BATTERY_MAX_SCALED_READING;
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

#ifdef BATTERY_MODE_ENABLED
	// Reads the battery voltage
	readBatteryVoltage();
#endif

	if (detectCapTouch()) {
		capacitiveTouchDetected = DETECTION_TRUE;
	} else {
		capacitiveTouchDetected = DETECTION_FALSE;
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

void fsmBatteryVoltage() {
	if (batteryVoltage >= 8.1) {
		batteryVoltageLevel = BATTERY_HIGH;
	} 
	else if (batteryVoltage >= 7.2) {
		batteryVoltageLevel = BATTERY_MED;
	}
	else if (batteryVoltage >= 6.3) {
		batteryVoltageLevel = BATTERY_LOW;
	} else {
		batteryVoltageLevel = BATTERY_DEAD; 
	}
}

void fsmCapacitiveTouch() {
	switch (capacitiveTouchDetected) {
		case DETECTION_FALSE:
			// If there is no capacitive touch, we only need to do something if
			// it was just released
			if (capState == CAP_PRESSED)
				capState = CAP_RELEASED;
			break;
		case DETECTION_TRUE:
			// If the capacitive touch is detected, we want to wait till it is released
			if (capState == CAP_WAITING)
				capState = CAP_PRESSED;
			break;
	}
}

void RobotPlanning() {
	fsmCollisionDetection();
	fsmTempLightDetection();
	fsmServoMovement();
#ifdef BATTERY_MODE_ENABLED
	fsmBatteryVoltage();
#endif
	fsmCapacitiveTouch();
}

// ================================ ACTION STATE FUNCTIONS ======================================

void RobotAction() {
	uint8_t status;
    handleCollisionAction();

	handleDriveAction();

	handleServoAction();

#ifdef BATTER_MODE_ENABLED
	handleBatteryLEDAction();
#endif

	handleCapacitiveTouchAction();

	
#ifdef DEBUG_MODE
	debugRobotState();
#endif
}	

void debugRobotState() {
	// printRobotState(&detectedData, &actionStates);

	// sendPinData(BUTTON_SERVO_DOWN, readPinVoltage(BUTTON_SERVO_DOWN));
	// sendPinData(BUTTON_SERVO_UP, readPinVoltage(BUTTON_SERVO_UP));
	// sendPinData(BUTTON_COLLISION, readPinVoltage(BUTTON_COLLISION));
	
	Serial.print("Battery voltage: ");
	Serial.println(batteryVoltage);
}

void handleDriveAction() {
	int pin;
	#ifdef LED_DEBUG_MODE
	pin = LED_MOTOR_LEFT;
	#else
	pin = MOTOR_LEFT;
	#endif

	// If DRIVE_LEFT flag set, drive left
	if (actionStates.Drive & DRIVE_LEFT) {
		analogWrite(pin, robotSpeed);
	} else {
		analogWrite(pin, 0);
	}
	
	#ifdef LED_DEBUG_MODE
	pin = LED_MOTOR_RIGHT;
	#else
	pin = MOTOR_RIGHT;
	#endif

	// If DRIVE_RIGHT flag set, drive right
	if (actionStates.Drive & DRIVE_RIGHT) {
		int motorBalanceFactor = 0;
		if (robotSpeed != STOPPED)
			motorBalanceFactor = RIGHT_MOTOR_BALANCE_FACTOR;


		analogWrite(pin, robotSpeed - motorBalanceFactor);

	} else {
		analogWrite(pin, 0);
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

void handleBatteryLEDAction() {
	int pinValues[] = {LOW, LOW, LOW};

	switch (batteryVoltageLevel) {
		case BATTERY_DEAD:
			break;
		case BATTERY_HIGH:
			pinValues[2] = HIGH;  // fall through
		case BATTERY_MED:  
			pinValues[1] = HIGH;  // fall through
		case BATTERY_LOW:
			pinValues[0] = HIGH;
			break;
	}

	digitalWrite(LED_BATT_LOW, pinValues[0]);
	digitalWrite(LED_BATT_MED, pinValues[1]);
	digitalWrite(LED_BATT_HIGH, pinValues[2]);
}

void activateLED(uint8_t ledPin) {
	digitalWrite(ledPin, HIGH);
}

void handleCapacitiveTouchAction() {
	if (capState == CAP_RELEASED) {
		toggleRobotSpeed();
		capState = CAP_WAITING;
	}
}

void toggleRobotSpeed() {
	switch (robotSpeed) {
		case STOPPED:
			robotSpeed = SLOW;
			break;
		case SLOW:
			robotSpeed = MEDIUM;
			break;
		case MEDIUM:
			robotSpeed = FAST;
			break;
		case FAST:
			robotSpeed = STOPPED;
			break;
	}

	Serial.print("robotSpeed: ");
	Serial.println(robotSpeed);
}

void disableLED(uint8_t ledPin) {
	digitalWrite(ledPin, LOW);
}
