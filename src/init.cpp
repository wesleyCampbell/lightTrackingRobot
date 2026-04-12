/**
 * @file 	init.cpp
 * @brief	Implemenation of functions defined in init.h
 *
 * Initializes all process and hardware requred for robot function.
 *
 * This file is part of the lightTrackingRobot project for the BYU ECEN240 course.
 *
 * @author 	Wesley Campbell
 * @date 	2026-01-16
 * @version	v1.0.3
 */

#include "init.h"
#include "capacitive_touch.h"
#include "params.h"

Servo servo;
int servoAngle;

NewPing sonarSensor(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_MAX_DIST);

void initPins() {
	// Input pins
	pinMode(BUTTON_COLLISION, INPUT);

	pinMode(MOTOR_LEFT, OUTPUT);
	pinMode(MOTOR_RIGHT, OUTPUT);

	pinMode(LED_COLLISION, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(CAP_OUT_PIN, OUTPUT);
	pinMode(CAP_IN_PIN, INPUT);

	pinMode(PHOTODIODE_TOP_LEFT, INPUT);
	pinMode(PHOTODIODE_BOTTOM_LEFT, INPUT);
	pinMode(PHOTODIODE_BOTTOM_RIGHT, INPUT);
	pinMode(PHOTODIODE_TOP_RIGHT, INPUT);

	// pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
	// pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}

void initSerialComm() {
	// Primary serial port
	Serial.begin(9600);  
}

void initServo() {
	servo.attach(SERVO_PIN);
	servo.write(SERVO_ANGLE_START);

	servoAngle = SERVO_ANGLE_START;
}
