/**
 * @file communicate.h
 *
 * @brief Contains methods usefull for sending information across the Serial Port
 *
 * Part of the lightTrackingRobot project for the BYU ECEN240 course.
 *
 * @author 	Wesley Campbell
 * @date 	2026-01-21
 * @version	1.0.1
 */


#ifndef __COMMUNICATE_H__
#define __COMMUNICATE_H__

#include "includes.h"
#include "robot_states.h"

#define SENSOR_DATA_BLOB_HEADER 0xAA
#define ACTION_DATA_BLOB_HEADER 0xBB
#define PIN_DATA_BLOB_HEADER 0xCC

#define DATA_BLOB_DATA_TYPE uint64_t
#define DATA_BLOB_DATA_SIZE (sizeof(DATA_BLOB_DATA_TYPE))

#define COMM_STATUS uint8_t
#define COMM_STATUS_OK 0
#define COMM_STATUS_FAIL 1
#define STATUS_DATA_BLOB_FILLED 2

#define NEW_DATA_BLOB() (struct dataBlob*)malloc(sizeof(struct dataBlob))


/*
 * @brief A structure used to form binary large objects
 */
struct dataBlob {
	DATA_BLOB_DATA_TYPE dataBlob;
	uint8_t header;
	uint8_t dataUsed; 
};

/*
 * @brief Creates a new, empty dataBlob
 *
 * @return dataBlob*. Must free when done
 */
void initializeBlob(struct dataBlob* blob);

/*
 * @brief Creates a new, empty dataBlob
 *
 * @return dataBlob*. Must free when done
 */
struct dataBlob* newActionDataBlob();

/*
 * @brief Creates a new, empty dataBlob
 *
 * @return dataBlob*. Must free when done
 */
struct dataBlob* newSensorDataBlob();

/*
 * @brief Creates a new, empty dataBlob
 *
 * @return dataBlob*. Must free when done
 */
struct dataBlob* newPinDataBlob();

/*
 * @brief Marshalls a byte of data into the next open position in a dataBlob object
 *
 * @param dataBlob A pointer to the dataBlob
 * @param data The byte of data
 *
 * @return A status code indicating success or failure
 */
COMM_STATUS dataMarshall_uint8(struct dataBlob* dataBlob, uint8_t data);

/*
 * @brief Marshalls two bytes of data into the next open position in a dataBlob object
 *
 * @param dataBlob A pointer to the dataBlob
 * @param data The byte of data
 *
 * @return A status code indicating success or failure
 */
COMM_STATUS dataMarshall_uint16(struct dataBlob* dataBlob, uint8_t data);

/*
 * @brief Sends marshelled data held within a dataBlob object
 *
 * @param dataBlob* A pointer to the dataBlob
 * 
 * @return A status code indicating success or failure
 */
COMM_STATUS sendMarshalledData(struct dataBlob* dataBlob);

/**
 * @brief Marshalls analogPin data and sends it down the wire.
 *
 * @param pin The pin being read
 * @param data The data to send over the wire
 *
 * @return Status code indicating success or failure.
 */
COMM_STATUS sendPinData(uint8_t pin, uint16_t data);

/*
 * @brief a shorter version of Serial.println.
 *
 * @param msg A pointer to the string mesage
 */
void println(char* msg);

/*
 * @brief A shorter version of Serial.print
 *
 * @param msg A pointer to the string mesage
 */
void print(char* msg);

/*
 * @brief Prints a new line on the Serial appended with "DEBUG: "
 *
 * @param msg A pointer to the string mesage
 */
void debug(char* msg);

/*
 * @brief sends the robot data found within a detectionDataStruct down the wire
 *
 * @param data* The pointer to the data struct
 */
void printRobotData(detectionDataStruct* data);

/*
 * @brief Sends the current robot action states the wire.
 *
 * @param actions* A pointer to the robot's actionStateStruct
 */
void printRobotActions(actionStateStruct* actions);

/*
 * @brief Sends both the robot's data and action states down the wire
 *
 * @param data* A pointer to the robot's data struct
 * @param actions* A pointer to the robot's action struct
 */
void printRobotState(detectionDataStruct* data, actionStateStruct* actions);

#endif  // __COMMUNICATE_H__
