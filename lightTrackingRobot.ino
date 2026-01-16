/**
 * @file lightTrackingRobot.ino
 * @brief	Launches the lightTrackingRobot program.
 *
 * @author	Wesley Campbell
 * @date 	2026-01-15
 * @version	v1.0.0
 */

#include "includes.h"

void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);
  initPins();

}

void loop() {
  activateLED((uint8_t)LED_BUILTIN);
  delay(900);
  disableLED((uint8_t)LED_BUILTIN);
  delay(600);
}
