/*
 * dictionary.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: ofacklam
 */


#include "dictionary.hpp"
#include "odTasks.h"
#include "can/canInterface.h"

#include <cstring>
#include <cmsis_os2.h>

/**
 * Object dictionary field allocation
 */
// https://stackoverflow.com/a/11322287
class OdFields {
public:
	OdFields() {
		ALLOCATE_OD_FIELD(ACCEL_A);
		ALLOCATE_OD_FIELD(ACCEL_B);
		ALLOCATE_OD_FIELD(OMEGA_A);
		ALLOCATE_OD_FIELD(OMEGA_B);
		ALLOCATE_OD_FIELD(PRESSURE_A);
		ALLOCATE_OD_FIELD(PRESSURE_B);

		ALLOCATE_OD_FIELD(ACCELERATION);
		ALLOCATE_OD_FIELD(OMEGA);
		ALLOCATE_OD_FIELD(PRESSURE);

		ALLOCATE_OD_FIELD(GNSS_DATA);

		ALLOCATE_OD_FIELD(ROCKET_STATE);

		ALLOCATE_OD_FIELD(STATUS);
	}

	ComFrame &operator[](uint8_t index) {
		return fields[index];
	}

private:
	ComFrame fields[OD_MAX_DATAID];
};

static OdFields dict;


/**
 * Read/write interface
 */
template<uint8_t DATAID>
void readOD(typename DataIdToType<DATAID>::type *data) {
	int32_t lock = osKernelLock();

	ComFrame &f = dict[DATAID];
	std::memcpy((uint8_t*) data, f.data, f.size);

	osKernelRestoreLock(lock);
}

template<uint8_t DATAID>
void writeOD(typename DataIdToType<DATAID>::type *data) {
	ComFrame toSend;
	toSend.dataID = dict[DATAID].dataID;
	toSend.size = dict[DATAID].size;
	std::memcpy(toSend.data, (uint8_t*) data, toSend.size);

	canPushToOutQ(&toSend);
}


/**
 * Task definition
 */
void odUpdateTask(void *argument) {
	while(true) {
		// Get latest incoming frame (blocking)
		ComFrame toReceive;
		canPopFromInQ(&toReceive);

		// Update field atomically
		int32_t lock = osKernelLock();

		ComFrame &toUpdate = dict[toReceive.dataID];
		std::memcpy(toUpdate.data, toReceive.data, toUpdate.size);

		osKernelRestoreLock(lock);
	}
}

