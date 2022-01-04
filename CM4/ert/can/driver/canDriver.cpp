/*
 * canDriver.cpp
 *
 *  Created on: Jan 1, 2022
 *      Author: ofacklam
 */

#include "canDriver.hpp"

/**
 * Global references to all CAN cores, set up during driver initialization
 * Necessary to allow Interrupt -> Driver routing
 */
CanCore* cores[CAN_MAX_CORES];
uint8_t nbCanCores = 0;

/**
 * Interrupts
 */
extern "C"
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes) {
	// messages have been transmitted
}

extern "C"
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if(RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
		// new messages received
		for(uint8_t c = 0; c < nbCanCores; c++)
			cores[c]->receiveCallback(hfdcan);
	}
}

extern "C"
void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan) {
	// protocol error occurred
}

extern "C"
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
	// bus error occurred
}

/**
 * Driver
 */
CanCore::CanCore(FDCAN_HandleTypeDef *phfdcan) : phfdcan(phfdcan) {
	// Peripheral, clocks & baud rate already initialized

	// Configure RX behavior
	HAL_FDCAN_ConfigGlobalFilter(phfdcan,
			FDCAN_ACCEPT_IN_RX_FIFO0,
			FDCAN_ACCEPT_IN_RX_FIFO0,
			FDCAN_REJECT_REMOTE,
			FDCAN_REJECT_REMOTE);
	HAL_FDCAN_ConfigRxFifoOverwrite(phfdcan, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE);

	// Configure interrupts
	uint32_t interrupts = FDCAN_IT_TX_COMPLETE
			| FDCAN_IT_RX_FIFO0_NEW_MESSAGE
			| FDCAN_IT_ARB_PROTOCOL_ERROR
			| FDCAN_IT_DATA_PROTOCOL_ERROR
			| FDCAN_IT_ERROR_PASSIVE
			| FDCAN_IT_ERROR_WARNING
			| FDCAN_IT_BUS_OFF;
	uint32_t allBuffers = 0xFFFFFFFF;
	HAL_FDCAN_ActivateNotification(phfdcan, interrupts, allBuffers);

	// Register CAN core globally
	cores[nbCanCores++] = this;

	// Start FDCAN
	HAL_FDCAN_Start(phfdcan);
}

// TODO: optimize frame size
void CanCore::send(uint8_t src, ComFrame *frame) {
	// check free level
	uint32_t freeLevel = HAL_FDCAN_GetTxFifoFreeLevel(phfdcan);
	if(freeLevel <= 0)
		return;

	// send frame
	FDCAN_TxHeaderTypeDef txHeader = {
			.Identifier = (frame->dataID << CAN_DATAID_OFFSET) | (src & CAN_BOARDID_MASK),
			.IdType = FDCAN_STANDARD_ID, // 11 bits
			.TxFrameType = FDCAN_DATA_FRAME,
			.DataLength = FDCAN_DLC_BYTES_64,
			.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
			.BitRateSwitch = FDCAN_BRS_OFF,
			.FDFormat = FDCAN_FD_CAN,
			.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
	};
	HAL_FDCAN_AddMessageToTxFifoQ(phfdcan, &txHeader, frame->data);
}

void CanCore::receiveCallback(FDCAN_HandleTypeDef *interruptSrc) {
	// check receiving core
	if(interruptSrc->Instance != phfdcan->Instance)
		return;

	// drain the FIFO
	while(HAL_FDCAN_GetRxFifoFillLevel(phfdcan, FDCAN_RX_FIFO0) > 0) {
		ComFrame frame;
		FDCAN_RxHeaderTypeDef rxHeader;

		HAL_FDCAN_GetRxMessage(phfdcan, FDCAN_RX_FIFO0, &rxHeader, frame.data);

		frame.dataID = rxHeader.Identifier >> CAN_DATAID_OFFSET;
		uint8_t source = rxHeader.Identifier & CAN_BOARDID_MASK;

		if(rxHandler != nullptr)
			rxHandler(source, &frame);

		//consoleLog("[BOARD %d] Received DATA_ID %d: %2x %2x %2x %2x %2x %2x %2x %2x\n", source, frame.dataID,
		//		frame.data[0], frame.data[1], frame.data[2], frame.data[3],
		//		frame.data[4], frame.data[5], frame.data[6], frame.data[7]);
	}
}
