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

void initPins() {
	// Input pins
	pinMode(BUTTON_COLLISION, INPUT);
	
	// Output pins
	pinMode(LED_SERVO_UP, OUTPUT);
	pinMode(LED_SERVO_DOWN, OUTPUT);

	pinMode(MOTOR_LEFT, OUTPUT);
	pinMode(MOTOR_RIGHT, OUTPUT);
	pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
	pinMode(MOTOR_LEFT_REVERSE, OUTPUT);
	pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
	pinMode(MOTOR_RIGHT_REVERSE, OUTPUT);

	pinMode(LED_COLLISION, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(CAP_OUT_PIN, OUTPUT);
	pinMode(CAP_IN_PIN, INPUT);

	pinMode(PHOTODIODE_TOP_LEFT, INPUT);
	pinMode(PHOTODIODE_BOTTOM_LEFT, INPUT);
	pinMode(PHOTODIODE_BOTTOM_RIGHT, INPUT);
	pinMode(PHOTODIODE_TOP_RIGHT, INPUT);
}

void initSerialComm() {
	// Primary serial port
	Serial.begin(9600);  
}
