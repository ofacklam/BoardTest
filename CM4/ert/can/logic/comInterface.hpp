/*
 * comInterface.hpp
 *
 *  Created on: Jan 1, 2022
 *      Author: ofacklam
 */

#ifndef CAN_LOGIC_COMINTERFACE_HPP_
#define CAN_LOGIC_COMINTERFACE_HPP_

#include <can/canInterface.h>

typedef void (*ReceptionHandler)(uint8_t src, ComFrame *frame);

class ComInterface {

public:
	// Constructor
	ComInterface() : rxHandler(nullptr) {}

	// Upper interface
	virtual void send(uint8_t src, ComFrame *frame) = 0;
	void registerReceptionHandler(ReceptionHandler rxh) {
		rxHandler = rxh;
	}

protected:
	ReceptionHandler rxHandler;

};



#endif /* CAN_LOGIC_COMINTERFACE_HPP_ */
