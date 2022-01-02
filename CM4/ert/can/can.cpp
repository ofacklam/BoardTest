/*
 * can.cpp
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

/*******
 * TEMP FILE -- TO BE REWORKED
 */

#include "can/can.h"
#include "can/logic/comInterface.hpp"
#include "stm32mp1xx_hal.h"

osMessageQueueId_t outQ;
osMessageQueueId_t inQ;

extern FDCAN_HandleTypeDef hfdcan1;

void canPushToOutQ(ComFrame *frame) {
	osMessageQueuePut(outQ, frame, 0U, 0U);
}

void canSetupQueues() {
	osMessageQueueAttr_t outAttr = {
			.name = "CAN outgoing queue",
	};
	outQ = osMessageQueueNew(CAN_MSGQ_SIZE, sizeof(ComFrame), &outAttr);
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes) {
	// messages have been transmitted
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if(RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
		// new messages received => drain the FIFO
		while(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0) {
			ComFrame frame;
			FDCAN_RxHeaderTypeDef rxHeader;
			HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rxHeader, frame.data);

			frame.dataID = rxHeader.Identifier >> 3;
			uint8_t source = rxHeader.Identifier & 0x07;

			printf("[BOARD %d] Received DATA_ID %d: %2x %2x %2x %2x %2x %2x %2x %2x\n", source, frame.dataID,
					frame.data[0], frame.data[1], frame.data[2], frame.data[3],
					frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
		}
	}
}

void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan) {
	// protocol error occurred
}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
	// bus error occurred
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

	// Configure FDCAN1
	HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,
			FDCAN_ACCEPT_IN_RX_FIFO0,
			FDCAN_ACCEPT_IN_RX_FIFO0,
			FDCAN_REJECT_REMOTE,
			FDCAN_REJECT_REMOTE);
	HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE);

	uint32_t interrupts = FDCAN_IT_TX_COMPLETE
			| FDCAN_IT_RX_FIFO0_NEW_MESSAGE
			| FDCAN_IT_ARB_PROTOCOL_ERROR
			| FDCAN_IT_DATA_PROTOCOL_ERROR
			| FDCAN_IT_ERROR_PASSIVE
			| FDCAN_IT_ERROR_WARNING
			| FDCAN_IT_BUS_OFF;
	uint32_t allBuffers = 0xFFFFFFFF;
	HAL_FDCAN_ActivateNotification(&hfdcan1, interrupts, allBuffers);

	// Start FDCAN1
	HAL_FDCAN_Start(&hfdcan1);

	// Infinite loop
	while(true) {
		// retrieve next frame
		ComFrame frame;
		osMessageQueueGet(outQ, &frame, NULL, 0U);

		// check free level
		uint32_t freeLevel = HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1);
		if(freeLevel > 0) {

			// send frame
			FDCAN_TxHeaderTypeDef txHeader = {
					.Identifier = frame.dataID << 3U | CAN_ID, // 8-bit data ID | 3-bit board ID
					.IdType = FDCAN_STANDARD_ID, // 11 bits
					.TxFrameType = FDCAN_DATA_FRAME,
					.DataLength = FDCAN_DLC_BYTES_64,
					.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
					.BitRateSwitch = FDCAN_BRS_OFF,
					.FDFormat = FDCAN_FD_CAN,
					.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
			};
			HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, frame.data);

		}
	}
}

