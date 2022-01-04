/*
 * canDriver.h
 *
 *  Created on: Jan 1, 2022
 *      Author: ofacklam
 */

#ifndef CAN_CANDRIVER_H_
#define CAN_CANDRIVER_H_

#include "can/logic/comInterface.hpp"
#include "stm32mp1xx_hal.h"

#define CAN_MAX_CORES 2
#define CAN_DATAID_OFFSET 3U 	// 8-bit data ID | 3-bit board ID
#define CAN_BOARDID_MASK 0x07U

class CanCore : public ComInterface {

public:
	CanCore(FDCAN_HandleTypeDef *phfdcan);
	void send(uint8_t src, ComFrame *frame);

	// Bottom interface
	void receiveCallback(FDCAN_HandleTypeDef *interruptSrc);

private:
	FDCAN_HandleTypeDef *phfdcan;
};

#endif /* CAN_CANDRIVER_H_ */
