/*
 * can.cpp
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */


#include "can/canInterface.h"
#include "can/canTasks.h"
#include "can/driver/canDriver.hpp"
#include "debug/console.h"

#include "stm32mp1xx_hal.h"
#include <cmsis_os2.h>

/**
 * Synchronization primitives
 */
osMessageQueueId_t outQ;
osMessageQueueId_t inQ;

/**
 * Peripheral pointers
 */
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

/**
 * CAN interface
 */
void canPushToOutQ(ComFrame *frame) {
	osMessageQueuePut(outQ, frame, 0U, osWaitForever);
}

void canPopFromInQ(ComFrame *frame) {
	osMessageQueueGet(inQ, frame, NULL, osWaitForever);
}

/**
 * CAN task definitions
 */
void canSetupQueues() {
	osMessageQueueAttr_t outAttr = {
			.name = "CAN outgoing queue",
	};
	outQ = osMessageQueueNew(CAN_MSGQ_SIZE, sizeof(ComFrame), &outAttr);

	osMessageQueueAttr_t inAttr = {
			.name = "CAN incoming queue",
	};
	inQ = osMessageQueueNew(CAN_MSGQ_SIZE, sizeof(ComFrame), &inAttr);
}

void canReceiveHandler(uint8_t src, ComFrame *frame) {
	osMessageQueuePut(inQ, frame, 0U, 0U);
}

void canTransmitTask(void *argument) {

	// Configure clock unit --> route kernel clock directly to CAN core
	// Kernel clock & bit timings are determined by settings in CubeMX
	// f_tqck = f_kerck = 32Mhz
	// tq = prescaler * t_tqck = prescaler / f_tqck
	// t_sync = 1 tq; t_bs1 = segment1 * tq; t_bs2 = segment2 * tq
	// t_bit = t_sync + t_bs1 + t_bs2 = tq * (segment1 + segment2 + 1)
	// baudrate = f_tqck / (prescaler * (seg1 + seg2 + 1)) = 1Mbps
	FDCAN_ClkCalUnitTypeDef ccuConfig = {
			.ClockCalibration = FDCAN_CLOCK_CALIBRATION_DISABLE,
			.ClockDivider = FDCAN_CLOCK_DIV1,
	};
	HAL_FDCAN_ConfigClockCalibration(&hfdcan1, &ccuConfig);

	// Initialize FDCAN drivers
	CanCore fdcan1(&hfdcan1);
	fdcan1.registerReceptionHandler(canReceiveHandler);

	// Infinite loop
	while(true) {
		// retrieve next frame
		ComFrame frame;
		osMessageQueueGet(outQ, &frame, NULL, osWaitForever);

		// send over CAN
		fdcan1.send(CAN_ID, &frame);
	}
}

