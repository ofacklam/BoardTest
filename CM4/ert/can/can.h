/*
 * can.h
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#ifndef CAN_CAN_H_
#define CAN_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <cmsis_os2.h>

#define CAN_ID 0U
#define CAN_MSGQ_SIZE 32
#define FRAME_MAX_SIZE 64

typedef struct {
	uint8_t dataID;
	uint8_t size;
	uint8_t data[FRAME_MAX_SIZE];
} ComFrame;

void canPushToOutQ(ComFrame *frame);
void canPopFromInQ(ComFrame *frame);

void canSetupQueues();
void canTransmitTask(void *argument);


#ifdef __cplusplus
}
#endif

#endif /* CAN_CAN_H_ */
