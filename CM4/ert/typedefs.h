/*
 * typedefs.h
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_


typedef float acceleration_t;

typedef struct {
	acceleration_t x;
	acceleration_t y;
	acceleration_t z;
} acceleration3D_t;


typedef float angularvelocity_t;

typedef struct {
	angularvelocity_t x;
	angularvelocity_t y;
	angularvelocity_t z;
} angularvelocity3D_t;


typedef float pressure_t;


#endif /* TYPEDEFS_H_ */
