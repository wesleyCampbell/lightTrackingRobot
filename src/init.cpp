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
 * @version	v1.0.1
 */

#include "init.h"

void initPins() {
	// Input pins
	pinMode(BUTTON_SERVO_UP, INPUT);
	pinMode(BUTTON_SERVO_DOWN, INPUT);
	pinMode(BUTTON_MOTOR_LEFT, INPUT);
	pinMode(BUTTON_MOTOR_RIGHT, INPUT);
	pinMode(BUTTON_COLLISION, INPUT);
	
	// Output pins
	pinMode(LED_SERVO_UP, OUTPUT);
	pinMode(LED_SERVO_DOWN, OUTPUT);
	pinMode(LED_MOTOR_LEFT, OUTPUT);
	pinMode(LED_MOTOR_RIGHT, OUTPUT);
	pinMode(LED_COLLISION, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
}

void initSerialComm() {
	// Primary serial port
	Serial.begin(9600);  
}
