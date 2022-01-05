/*
 * dictionary.hpp
 *
 *  Created on: Jan 5, 2022
 *      Author: ofacklam
 */

#ifndef OD_DICTIONARY_HPP_
#define OD_DICTIONARY_HPP_

#include "odMacros.hpp"
#include "typedefs.h"

#define OD_MAX_DATAID 256 // 8-bit ID
#define MAX_SENSORS_PER_BOARD 2

/**
 * Object dictionary type declarations
 */
REGISTER_OD_FIELD(2U, ACCEL_A, acceleration3D_t[MAX_SENSORS_PER_BOARD])
REGISTER_OD_FIELD(3U, ACCEL_B, acceleration3D_t[MAX_SENSORS_PER_BOARD])
REGISTER_OD_FIELD(4U, OMEGA_A, angularvelocity3D_t[MAX_SENSORS_PER_BOARD])
REGISTER_OD_FIELD(5U, OMEGA_B, angularvelocity3D_t[MAX_SENSORS_PER_BOARD])
REGISTER_OD_FIELD(6U, PRESSURE_A, pressure_t[MAX_SENSORS_PER_BOARD])
REGISTER_OD_FIELD(7U, PRESSURE_B, pressure_t[MAX_SENSORS_PER_BOARD])

REGISTER_OD_FIELD(8U, ACCELERATION, acceleration3D_t)
REGISTER_OD_FIELD(9U, OMEGA, angularvelocity3D_t)
REGISTER_OD_FIELD(10U, PRESSURE, pressure_t)

REGISTER_OD_FIELD(11U, GNSS_DATA, gnss_t)

REGISTER_OD_FIELD(12U, ROCKET_STATE, rocketstate_t)

REGISTER_OD_FIELD(13U, STATUS, status_t)


/**
 * Interface
 * !! DO NOT CALL FROM ISR !!
 */
void __unsafe_readOD(uint8_t dataID, uint8_t *data);

template<uint8_t DATAID>
void readOD(typename DataIdToType<DATAID>::type *data) {
	__unsafe_readOD(DATAID, (uint8_t*) data);
}

void __unsafe_writeOD(uint8_t dataOD, uint8_t *data);

template<uint8_t DATAID>
void writeOD(typename DataIdToType<DATAID>::type *data) {
	__unsafe_writeOD(DATAID, (uint8_t*) data);
}


#endif /* OD_DICTIONARY_HPP_ */
