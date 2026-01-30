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
 * @version	v1.0.1
 **/

//======================== PIN DECLARATIONS ===============================

#ifndef __PARAMS_H__
#define __PARAMS_H__

// Button input pins
#define BUTTON_SERVO_UP    A2
#define BUTTON_SERVO_DOWN  A5
#define BUTTON_MOTOR_LEFT  A3
#define BUTTON_MOTOR_RIGHT A6
#define BUTTON_COLLISION   A4

// LED output pins
#define LED_SERVO_UP	6
#define LED_SERVO_DOWN	5
#define LED_MOTOR_LEFT	2
#define LED_MOTOR_RIGHT	4
#define LED_COLLISION	3

// Motor output pins
// TODO: implement motors

// Photodiode input pins
// TODO: implement this

// Capacitive input pins
// TODO: implement this

// Ultrasonic input pin
// TODO: implement this

// Servo output pin
// TOOD: implement this

// ======================= SAMPLING PARAMETERS =============================

// The voltage threshold at which a button is pressed
#define BUTTON_VTHRESHOLD 3.5 

#define VOLTAGE_MAX 5
#define SENSOR_MAX_OUT 1024

#endif  // __PARAMS_H__
