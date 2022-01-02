/*
 * threads.c
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#include "can/can.h"

void testThread(void *argument) {
	uint8_t counter = 0;

	while(1) {
		ComFrame frame;
		for(uint8_t i = 0; i < 8; i++) {
			frame.data[i] = counter + i;
		}

		canPushToOutQ(&frame);
		canPushToOutQ(&frame);

		counter++;
		osDelay(1000);
	}
}

void createQueues() {
	canSetupQueues();
}

void createThreads() {
	osThreadNew(canTransmitTask, NULL, NULL);
	osThreadNew(testThread, NULL, NULL);
}

