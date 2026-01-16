# Light Tracking Robot

### A BYU ECEN240 Project

- Author: Wesley Campbell
- Date: 2026-01-16
- Version: v1.0.0

---

## Summary

Implementation of a finite-state-machine written in C in the Arduino Microprocessor format.
The purpose of the code is to program a robot to track and follow a light source using 
photo-diodes. The robot has several different states that define its function at any given moment.

## Dependancies

To compile and use the code, one must have the `arduino-cli` command installed, as that is the 
tool used to compile the project.

## Usage

Using `arduino-cli`, one can use the provided Makefile. Before so doing, however, one must supply
the boards FQBN and the port the Arduino unit is connected to. Simply edit the `BOARD_FQBN=` and `PORT=` lines
in the Makefile to correct values and then run `make` to compile.

To upload code to the Arduino unit, simply run `make upload`.

## Hardware

The current implementation simply requires an LED, resistor, and wires.
A circuit diagram will be provided below further along the development cycle.

---

# ChangeLog

##### (2026-01-16) -- v1.0.0:
- Initialized the project structure
- Wrote Makefile
- Wrote boilerplate infrastructure for robot state machine
- Implemented basic version of collision detection
