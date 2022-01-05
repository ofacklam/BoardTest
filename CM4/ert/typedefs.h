/*
 * typedefs.h
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_


// Acceleration
typedef float acceleration_t;

typedef struct {
	acceleration_t x;
	acceleration_t y;
	acceleration_t z;
} acceleration3D_t;


// Angular velocity
typedef float angularvelocity_t;

typedef struct {
	angularvelocity_t x;
	angularvelocity_t y;
	angularvelocity_t z;
} angularvelocity3D_t;


// Pressure
typedef float pressure_t;


// GNSS
typedef struct {
	float latitude;
	float laterr;
	float longitude;
	float lonerr;
	float altitude;
	float alterr;
	float dop; // dilution of precision
} gnss_t;


// Position
typedef float position_t;

typedef struct {
	position_t x;
	position_t y;
	position_t z;
} position3D_t;


// Velocity
typedef float velocity_t;

typedef struct {
	velocity_t x;
	velocity_t y;
	velocity_t z;
} velocity3D_t;


// Rocket state
typedef struct {
	position3D_t position;
	velocity3D_t velocity;
	acceleration3D_t acceleration;
} rocketstate_t;


// Status
typedef enum {
	idle,
	arm,
	liftoff,
	coast,
	primary,
	secondary,
	touchdown,
} status_t;

#endif /* TYPEDEFS_H_ */
