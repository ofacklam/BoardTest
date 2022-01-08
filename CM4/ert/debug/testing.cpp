/*
 * testing.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: ofacklam
 */

#include "testing.h"
#include "console.h"
#include "od/dictionary.hpp"

#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <task.h>
#include "stm32mp1xx_hal.h"

void testThread(void *argument) {

	uint8_t counter = 0;
	char debug_buf[40*6];

	acceleration3D_t acc;
	acc.x = 123;
	acc.y = 456;
	acc.z = 789;

	int32_t start = HAL_GetTick();
	for(int i = 0; i < 32; i++) {
		writeOD<ACCELERATION>(&acc);
	}
	int32_t stop = HAL_GetTick();

	vTaskGetRunTimeStats(debug_buf);
	consoleLog("%s\n", debug_buf);
	consoleLog("Total time measured in test thread: %d\n", (stop - start));

	while(1) {
		//consoleLog("Loop iteration!\n");
		osDelay(1000);
	}
}


