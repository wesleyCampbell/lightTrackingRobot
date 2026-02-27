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
#define BUTTON_SERVO_UP    A3
#define BUTTON_SERVO_DOWN  A7
#define BUTTON_MOTOR_LEFT  A6
#define BUTTON_MOTOR_RIGHT A4
#define BUTTON_COLLISION   A5

// LED output pins
#define LED_SERVO_UP	2
#define LED_SERVO_DOWN	6
#define LED_MOTOR_LEFT	0
#define LED_MOTOR_RIGHT	0
#define LED_COLLISION	4

#define LED_BATT_LOW 7
#define LED_BATT_MED 9
#define LED_BATT_HIGH 11

// Uncomment line to enable LED Debug mode, where the motors are represented
// by LED lights on different pins instead.

// #define LED_DEBUG_MODE true

// Motor output pins
#define MOTOR_LEFT 5
#define MOTOR_RIGHT 3

// Battery Voltage Sensor Pin
#define BATTERY_PIN A2

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

#define BATTERY_MAX_VOLTAGE 9 
#define BATTERY_SCALED_MAX_VOLTAGE 3.877 
#define BATTERY_MAX_SCALED_READING (BATTERY_SCALED_MAX_VOLTAGE * SENSOR_MAX_OUT / VOLTAGE_MAX);
#endif  // __PARAMS_H__
