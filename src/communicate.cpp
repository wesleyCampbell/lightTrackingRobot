/**
 * @file communicate.c
 *
 * @brief Contains methods usefull for sending information across the Serial Port
 *
 * This file is part of the lightTrackingRobotProject for the BYU ECEN240 course.
 *
 * @author Wesley Campbell
 * @date 2026-01-26
 * @version 1.0.1
 */

#include "includes.h"

void println(char* msg) {
	Serial.println(msg);
}

void print(char* msg) {
	Serial.print(msg);
}

void debug(char* msg) {
	print("DEBUG: ");
	print(msg);
}

struct dataBlob* newSensorDataBlob() {
	struct dataBlob* outBlob = (struct dataBlob*)malloc(sizeof(struct dataBlob));

	outBlob->dataBlob = 0;
	outBlob->dataUsed = 0;
	outBlob->header = SENSOR_DATA_BLOB_HEADER;

	return outBlob;
}

struct dataBlob* newActionDataBlob() {
	struct dataBlob* outBlob = (struct dataBlob*)malloc(sizeof(struct dataBlob));

	outBlob->dataBlob = 0;
	outBlob->dataUsed = 0;
	outBlob->header = ACTION_DATA_BLOB_HEADER;

	return outBlob;
}

COMM_STATUS dataMarshall_uint8(struct dataBlob* dataBlob, uint8_t data) {
	// Check to see if the blob has room
	if (dataBlob->dataUsed >= DATA_BLOB_DATA_SIZE) {
		return STATUS_DATA_BLOB_FILLED;
	}

	// Format the package
	DATA_BLOB_DATA_TYPE package = (DATA_BLOB_DATA_TYPE) data << (8 * dataBlob->dataUsed);

	// Insert it into the correct location
	dataBlob->dataBlob |= package;

	// Update dataUsed pointer
	dataBlob->dataUsed++;

	return COMM_STATUS_OK;
}

COMM_STATUS sendMarshalledData(struct dataBlob* dataBlob) {
	Serial.write(dataBlob->header);

	for (int i = 0; i < dataBlob->dataUsed; i++) {
		uint8_t payload = dataBlob->dataBlob >> (i * 8);
		Serial.write(payload);
	}
}

uint8_t marshallLightData(detectionDataStruct* data) {
	uint8_t outData = 0;

	outData |= (data->lightDetected.down << 0);
	outData |= (data->lightDetected.left << 1);
	outData |= (data->lightDetected.right << 2);
	outData |= (data->lightDetected.up << 3);

	return outData;
}

void printRobotData(detectionDataStruct* data) {
	struct dataBlob* dataBlob = newSensorDataBlob();

	dataMarshall_uint8(dataBlob, marshallLightData(data));
	dataMarshall_uint8(dataBlob, data->collisionDetected);
	dataMarshall_uint8(dataBlob, data->capacitiveTouchDetected);

	sendMarshalledData(dataBlob);

	free(dataBlob);
}


void printRobotActions(actionStateStruct* actions) {
	struct dataBlob* actionBlob = newActionDataBlob();

	dataMarshall_uint8(actionBlob, actions->Collision);
	dataMarshall_uint8(actionBlob, actions->Drive);
	dataMarshall_uint8(actionBlob, actions->Servo);

	sendMarshalledData(actionBlob);

	free(actionBlob);
}

void printRobotState(detectionDataStruct* data, actionStateStruct* actions) {
	printRobotData(data);
	printRobotActions(actions);
}
