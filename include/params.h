/**
 * @file	params.h
 * @brief	Defines global paramaters for the lightTrackingRobot project.
 *
 * In this file are defined all the global-level parameters and variables used to control
 * functionality of the robot. This includes pin definitions, definitions of robot states, 
 * voltage thresholds, etc.
 *
 * This file is part of the lightTrackingRobot project for the BYU ECEN240 course.
 *
 * @author	Wesley Campbell
 * @date	2026-01-15	
 * @version	v1.0.2
 **/

//======================== PIN DECLARATIONS ===============================

#ifndef __PARAMS_H__
#define __PARAMS_H__

// #define DEBUG_MODE true

// Button input pins
#define BUTTON_COLLISION   A6

// LED output pins
#define LED_COLLISION	4

// Motor output pins
#define MOTOR_LEFT 5
#define MOTOR_RIGHT 3

// Photodiode input pins
#define PHOTODIODE_TOP_LEFT A0
#define PHOTODIODE_BOTTOM_LEFT A1
#define PHOTODIODE_BOTTOM_RIGHT A2
#define PHOTODIODE_TOP_RIGHT A3

// Ultrasonic input pin
#define ULTRASONIC_TRIGGER_PIN 10
#define ULTRASONIC_ECHO_PIN 8

// Servo output pin
#define SERVO_PIN 6
#define SERVO_ANGLE_MIN 95
#define SERVO_ANGLE_MAX 175
#define SERVO_ANGLE_START 150 
#define SERVO_ANGLE_DELTA 2

// ======================= SAMPLING PARAMETERS =============================

// The voltage threshold at which a button is pressed
#define BUTTON_VTHRESHOLD 3.5 

#define VOLTAGE_MAX 5.10
#define SENSOR_MAX_OUT 1024

// The max distance to consider for the ultrasonic sensor.
#define ULTRASONIC_MAX_DIST 200
#define COLLISION_DISTANCE 7

#endif  // __PARAMS_H__
