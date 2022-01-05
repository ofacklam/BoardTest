/*
 * dictionary.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: ofacklam
 */


#include "dictionary.hpp"
#include "can/canInterface.h"


/**
 * Object dictionary field allocation
 */
ComFrame fields[OD_MAX_DATAID] = {
		ALLOCATE_OD_FIELD(ACCEL_A, acceleration3D_t),
};


void odUpdateTask(void *argument) {
	acceleration3D_t test = readOD<ACCEL_A>();
	writeOD<ACCEL_A>(test);
}

