/*
 * dictionary.h
 *
 *  Created on: Jan 5, 2022
 *      Author: ofacklam
 */

#ifndef OD_DICTIONARY_HPP_
#define OD_DICTIONARY_HPP_

#include "odMacros.hpp"
#include "typedefs.h"

#define OD_MAX_DATAID 256 // 8-bit ID


/**
 * Object dictionary type declarations
 */
REGISTER_OD_FIELD(0, ACCEL_A, acceleration3D_t)


/**
 * Interface
 */
template<uint8_t DATAID>
typename DataIdToType<DATAID>::type readOD();

template<uint8_t DATAID>
void writeOD(typename DataIdToType<DATAID>::type data);


#endif /* OD_DICTIONARY_HPP_ */
