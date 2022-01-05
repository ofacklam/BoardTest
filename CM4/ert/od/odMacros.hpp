/*
 * odMacros.h
 *
 *  Created on: Jan 5, 2022
 *      Author: ofacklam
 */

#ifndef OD_ODMACROS_HPP_
#define OD_ODMACROS_HPP_


#include <stdint.h>

/**
 * Macros for Object Dictionary type definitions
 * TODO: assert type size < CAN frame size
 */
// https://stackoverflow.com/a/39837217
template<uint8_t DATAID> struct DataIdToType;

#define REGISTER_OD_FIELD(idNum, idString, typeClass) \
	static const uint8_t idString = idNum; \
	template<> struct DataIdToType<idString> { using type = typeClass; };

/**
 * Macro for Object Dictionary initialization
 */
#define ALLOCATE_OD_FIELD(idNum, typeClass) \
	[idNum] = { .dataID = idNum, .size = sizeof(typeClass), }


#endif /* OD_ODMACROS_HPP_ */
