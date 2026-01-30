/**
 * @file	robot_states.h
 * @brief	Robot state functionality and configuration.
 *
 * Robot states are defined as well as the functions that are used to calculate or
 * manage state control and functionality.
 * The robot itself will have three distinct phases that it will move through:
 * 		1. Detection
 * 		2. Planning
 * 		3. Action
 * Each of these states will have its own actions and logical flows, which are 
 * defined in this file
 *
 * This file is part of the lightTrackingRobot project for the BYU ECEN240 project.
 *
 * @author	Wesley Campbell
 * @date	2026-01-15
 * @version v1.0.1
 **/

#ifndef __ROBOT_STATES_H__
#define __ROBOT_STATES_H__

#include "includes.h"
#include <stdint.h>

//========================== STATE DEFINITIONS =====================================

// Detection phases 
#define DETECTION_FALSE	0
#define DETECTION_TRUE 	1

// Collision Phases
#define COLLISION_ACTIVE	1
#define COLLISION_INACTIVE 	0

// Driving Phases
#define DRIVE_STOP      0x00
#define DRIVE_LEFT      0x01
#define DRIVE_RIGHT     0x10
#define DRIVE_STRAIGHT  0x11

// Servo Movement Phases
#define SERVO_MOVE_STOP   0x00
#define SERVO_MOVE_UP	  0x01
#define SERVO_MOVE_DOWN   0x10
#define SERVO_MOVE_UP_DOWN  (SERVO_MOVE_UP | SERVO_MOVE_DOWN)


// ========================== STATE TRACKING VARIABLES ===============================

/*
 * @brief Struct used for storing robot data
 */
typedef struct _detectionDataStruct {
	struct {
		uint8_t right;
		uint8_t left;
		uint8_t up;
		uint8_t down; 
	} lightDetected;
	uint8_t collisionDetected;
	uint8_t capacitiveTouchDetected;
} detectionDataStruct;

/*
 * @brief Struct used for storing robot action states
 */
typedef struct _actionStateStruct {
	uint8_t Collision;
	uint8_t Drive;
	uint8_t Servo; 
} actionStateStruct;

#define NEW_DETECTION_DATA_STRUCT detectionDataStruct { \
										.lightDetected = { \
											.right = DETECTION_FALSE, \
											.left = DETECTION_FALSE, \
											.up = DETECTION_FALSE, \
											.down = DETECTION_FALSE, \
										}, \
										.collisionDetected = DETECTION_FALSE, \
										.capacitiveTouchDetected = DETECTION_FALSE, \
									}

#define NEW_ACTION_STATE_STRUCT actionStateStruct { \
									.Collision = COLLISION_INACTIVE, \
									.Drive = DRIVE_STOP, \
									.Servo = SERVO_MOVE_STOP, \
								}

// ========================== DETECTION STATE FUNCTIONS =============================

/**
 * @brief	Manages the **Detection phase** of the robot.
 *
 * Will poll all of the sensors, collecting and storing all the necessary data
 * into the data tracking variables. 
 **/
void RobotDetection();

/**
 * @brief	Returns the voltage of a given pin value.
 *
 * @param uint8_t pin : the pin to read
 *
 * @return float : the voltage value
 **/
float readPinVoltage(uint8_t pin);

/**
 * @brief	Checks to see if a given button is pressed.
 *
 * @param uint8_t button_pin : the pin of the button to be read
 *
 * @return bool : true if pressed, false otherwise
 **/
bool buttonPushed(uint8_t button_pin);

/**
 * @brief	Checks to see if there is an obstacle in front of robot.
 *
 * Checks the ultrasonic sensor to see if path forward is blocked by an object.
 *
 * @return bool : true if object detected, false otherwise
 **/
bool detectCollision();

// ========================== PLANING STATE FUNCTIONS ==============================

/**
 * @brief	Manages the **Planning Phase** of the robot.
 *
 * Will assign actions based on data collected during the *detection* phase.
 **/
void RobotPlanning();


/**
 * @brief	State machine for managing a detected collision.
 *
 * Sets action flag based upon current collision state:
 *    Collision Detected: stop the robot
 *    No collision detected: keep going
 */
void fsmCollisionDetection();

/**
 * @brief	State machine for managing robot steering control
 *
 * Sets movement action flag based upon current light detection:
 *     If light is left: move left
 *     If light is right: move right
 *     If light is ahead: move straight.
 **/
void fsmSteerRobot();

/**
 * @brief	State machine for managing servo movement control
 *
 * Sets servo movement flags based upon the current light detection.
 *     If light is up: move servo up
 *     If light is down: move servo down
 *     If light is ahead: do nothing
 */
void fsmServoMovement();

/**
 * @brief	State machine for managing the capacitive sensor control
 */
void fsmCapacitiveSensorSpeedControl();

/**
 * @brief	State machine for changing the current speed.
 */
void fsmChangeSpeed();


// ============================= ACTION STATE FUNCTIONS ======================================

/**
 * @brief	Manages the **Action Phase** of the robot.
 *
 * Based upon the action flags set during the planning phase, this phase will
 * undertake the cooresponding actions.
 */
void RobotAction();

/**
 * @brief	Handles collision execution logic
 *
 * Sets the collision LED.
 * 
 * @return status code if collision detected, 0 otherwise.
 */
void handleDriveAction();

/**
 * @brief	Handles collision execution logic
 *
 * Sets the collision LED.
 * 
 * @return status code if collision detected, 0 otherwise.
 */
void handleCollisionAction();

/**
 * @brief	Moves the servo motor.
 *
 * Will move the servo motor up or down, based upon the condition flags set.
 */
void handleServoAction();

/**
 * @brief	Will control the motors to drive in desired direction
 *
 */
void driveControl();

/**
 * @brief	Toggles LED lights
 */
void toggleLEDs();

/**
 * @brief 	Turns on an LED 
 *
 * @param 	uint8_t ledPin : the pin of the LED to turn on 
 */
void activateLED(uint8_t ledPin);

/**
 * @brief 	Turns of an LED
 *
 * @param 	uint8_t ledPin : the pin of the LED to disable
 */
void disableLED(uint8_t ledPin);

#endif  // __ROBOT_STATES_H__
