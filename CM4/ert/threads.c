/*
 * threads.c
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#include "can/canTasks.h"
#include "can/canInterface.h"
#include "debug/console.h"

#include <cmsis_os2.h>

void testThread(void *argument) {
	uint8_t counter = 0;

	while(1) {
		ComFrame frame;
		for(uint8_t i = 0; i < 8; i++) {
			frame.data[i] = counter + i;
		}

		consoleLog("Sending...\n");
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
	osThreadAttr_t canAttr = {
			.name = "CAN transmit task",
			.priority = osPriorityHigh,
	};
	osThreadNew(canTransmitTask, NULL, &canAttr);

	osThreadAttr_t testAttr = {
			.name = "Test task"
	};
	osThreadNew(testThread, NULL, &testAttr);
}

