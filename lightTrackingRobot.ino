/**
 * @file lightTrackingRobot.ino
 * @brief	Launches the lightTrackingRobot program.
 *
 * This file is part of the lightTrackingRobotProject for the BYU ECEN240 course.
 *
 * @author	Wesley Campbell
 * @date 	2026-01-15
 * @version	v1.0.1
 */

#include "includes.h"

void setup() {
  initPins();

  initSerialComm();
}

void loop() {
  RobotDetection();

  RobotPlanning();

  RobotAction();
}
