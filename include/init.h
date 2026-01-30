/**
 * @file	init.h
 * @brief	Provides functions that allow the initialization of the robot
 *
 * Part of the lightTrackingRobot project for the BYU ECEN240 course.
 *
 * @author	Wesley Campbell
 * @date	2026-01-16
 * @version v1.0.0
 */

#ifndef __INIT_H__
#define __INIT_H__

#include "includes.h"

/**
 * @brief 	Sets the pinMode for all pins used in the robot
 */
void initPins();

/**
 * @brief	Initializes the serial port
 */
void initSerialComm();

#endif  // __INIT_H__
