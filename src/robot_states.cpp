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

#include "Arduino.h"
#include "capacitive_touch.h"
#include "includes.h"
#include "lightDirection.h"
#include "params.h"
#include "robot_states.h"

detectionDataStruct detectedData;

actionStateStruct actionStates;

float batteryVoltage = 0;
BATTERY_LEVEL batteryVoltageLevel;

int capacitiveTouchDetected = DETECTION_FALSE;
CAP_STATE capState = CAP_WAITING;
ROBOT_SPEED robotSpeed = SLOW;

extern Servo servo;
extern int servoAngle;
extern NewPing sonarSensor;

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
	static unsigned long lastPing = 0;
	static int sonarDistance = 0;

	if (millis() - lastPing > 40) {
		lastPing = millis();
		sonarDistance = sonarSensor.ping_cm();  // returns 0 when distance too far
		Serial.println(sonarDistance);
	}

	if (sonarDistance != 0) {
		return sonarDistance <= COLLISION_DISTANCE;	
	}

	return false;
}

void checkLight() {
	LIGHT_DIR lightDir = detectLightDirection();

	if (lightDir & LIGHT_DOWN)
		detectedData.lightDetected.down = DETECTION_TRUE;
	else
		detectedData.lightDetected.down = DETECTION_FALSE;

	if (lightDir & LIGHT_UP)
		detectedData.lightDetected.up = DETECTION_TRUE;
	else
		detectedData.lightDetected.up = DETECTION_FALSE;

	if (lightDir & LIGHT_LEFT)
		detectedData.lightDetected.left = DETECTION_TRUE;
	else
		detectedData.lightDetected.left = DETECTION_FALSE;

	if (lightDir & LIGHT_RIGHT)
		detectedData.lightDetected.right = DETECTION_TRUE;
	else
		detectedData.lightDetected.right = DETECTION_FALSE;

}

void RobotDetection() {
	// Check for an immemant collision
	if (collisionDetected()) {
		detectedData.collisionDetected = DETECTION_TRUE;
	} else {
		detectedData.collisionDetected = DETECTION_FALSE;
	}

	checkLight();

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
	fsmCapacitiveTouch();
}

// ================================ ACTION STATE FUNCTIONS ======================================

void RobotAction() {
	uint8_t status;
    handleCollisionAction();

	handleDriveAction();

	handleServoAction();

	handleCapacitiveTouchAction();
}	

void enableMotors() {
	analogWrite(MOTOR_LEFT, robotSpeed);
	analogWrite(MOTOR_RIGHT, robotSpeed);
}

void disableMotors() {
	analogWrite(MOTOR_LEFT, LOW);
	analogWrite(MOTOR_RIGHT, LOW);
}

void turnLeft() {
	analogWrite(MOTOR_RIGHT, robotSpeed);
}

void turnRight() {
	analogWrite(MOTOR_LEFT, robotSpeed);
}

void driveStraight() {
	enableMotors();
}

void handleDriveAction() {
	if ((actionStates.Drive & (DRIVE_LEFT | DRIVE_RIGHT)) == (DRIVE_LEFT | DRIVE_RIGHT)) {
		driveStraight();
	} else if (actionStates.Drive & DRIVE_LEFT) {
		turnLeft();
	} else if (actionStates.Drive & DRIVE_RIGHT) {
		turnRight();
	} else {
		disableMotors();
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
		servoAngle -= SERVO_ANGLE_DELTA;
		if (servoAngle < SERVO_ANGLE_MIN) {
			servoAngle = SERVO_ANGLE_MIN;
		}
		servo.write(servoAngle);
	}
	// If SERVO_MOVE_UP flag set, move servo up
	if (actionStates.Servo & SERVO_MOVE_UP) {
		servoAngle += SERVO_ANGLE_DELTA;
		
		if (servoAngle > SERVO_ANGLE_MAX) {
			servoAngle = SERVO_ANGLE_MAX;
		}

		servo.write(servoAngle);
	}
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
}

void disableLED(uint8_t ledPin) {
	digitalWrite(ledPin, LOW);
}
