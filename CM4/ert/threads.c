/*
 * threads.c
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#include "can/canTasks.h"
#include "od/odTasks.h"
#include "debug/console.h"
#include "debug/testing.h"

#include <cmsis_os2.h>


void createQueues() {
	canSetupQueues();
}

void createThreads() {
	osThreadAttr_t canAttr = {
			.name = "CAN TX task",
			.priority = osPriorityHigh,
	};
	osThreadNew(canTransmitTask, NULL, &canAttr);

	osThreadAttr_t testAttr = {
			.name = "Test task",
			.stack_size = 1024,
	};
	osThreadNew(testThread, NULL, &testAttr);

	osThreadAttr_t odAttr = {
			.name = "OD update task",
			.priority = osPriorityHigh,
	};
	osThreadNew(odUpdateTask, NULL, &odAttr);
}

