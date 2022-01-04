/*
 * canInterface.h
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#ifndef CAN_CANINTERFACE_H_
#define CAN_CANINTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#define CAN_MSGQ_SIZE 32
#define CAN_FRAME_MAX_SIZE 64

typedef struct {
	uint8_t dataID;
	uint8_t size;
	uint8_t data[CAN_FRAME_MAX_SIZE];
} ComFrame;

void canPushToOutQ(ComFrame *frame);
void canPopFromInQ(ComFrame *frame);


#ifdef __cplusplus
}
#endif

#endif /* CAN_CANINTERFACE_H_ */
