/*
 * canTasks.h
 *
 *  Created on: Jan 2, 2022
 *      Author: ofacklam
 */

#ifndef CAN_CANTASKS_H_
#define CAN_CANTASKS_H_

#ifdef __cplusplus
extern "C" {
#endif


#define CAN_ID 0U

void canSetupQueues();
void canTransmitTask(void *argument);


#ifdef __cplusplus
}
#endif

#endif /* CAN_CANTASKS_H_ */
