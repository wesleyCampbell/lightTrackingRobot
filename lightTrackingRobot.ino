/**
 * @file lightTrackingRobot.ino
 * @brief	Launches the lightTrackingRobot program.
 *
 * @author	Wesley Campbell
 * @date 	2026-01-15
 * @version	v1.0.1
 */

#include "includes.h"

void setup() {
  initPins();

}

void loop() {
  RobotDetection();

  RobotPlanning();

  RobotAction();
}
