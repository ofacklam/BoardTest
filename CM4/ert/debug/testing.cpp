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

void testThread(void *argument) {

	uint8_t counter = 0;

	while(1) {
		acceleration3D_t acc;
		readOD<ACCELERATION>(&acc);
		consoleLog("Acceleration[x]: %d\n", (int) acc.x);
		consoleLog("Acceleration[y]: %d\n", (int) acc.y);
		consoleLog("Acceleration[z]: %d\n", (int) acc.z);

		acc.x = counter;
		acc.y = counter;
		acc.z = counter;

		consoleLog("Sending...\n");
		writeOD<ACCELERATION>(&acc);
		consoleLog("Done!\n");

		counter++;
		osDelay(1000);
	}
}


