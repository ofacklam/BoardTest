/*
 * console.c
 *
 *  Created on: Jan 3, 2022
 *      Author: ofacklam
 */

#include "console.h"
#include "stm32mp1xx_hal.h"

#include <stdio.h>
#include <stdarg.h>


extern UART_HandleTypeDef huart7;

// TODO: thread safety
void consoleLog(const char *format, ...) {
	va_list args;
	va_start(args, format);

	// https://en.cppreference.com/w/c/io/fprintf -> Notes
	int sz = vsnprintf(NULL, 0, format, args);
	char buf[sz + 1];
	vsnprintf(buf, sizeof buf, format, args);

	// send to UART7
	HAL_UART_Transmit(&huart7, buf, sizeof buf, HAL_MAX_DELAY);

	va_end(args);
}

