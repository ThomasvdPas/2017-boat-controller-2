/*
 * Xsens.h
 *
 *  Created on: 13 feb. 2014
 *      Author: boris
 */
#include "MODSERIAL.h"
#include "XsensDataInterpreter.h"
#include "exceptions.h"

#ifndef XSENS_H_
#define XSENS_H_

class Xsens {
private:
	MODSERIAL* _serial;
	void reset();
	XsensDataInterpreter *interpreter;

public:
	Thread *processThread;
	xsens_data_t xsensData;
	Semaphore *sem;
	Mutex *xsensData_mutex;

	Xsens(MODSERIAL*);
	void requestMessage();
	void transmitReset();
	XsensException getChar(uint8_t buffer[], int bufferCount, uint16_t* dataLength);
	XsensException xsensReset();
	XsensException waitForWakeUp();
	XsensException receiveMessage(uint8_t data[], uint8_t* dataLength, uint16_t);
	uint8_t getchar2();
	void thread();
	void printState();
};


#endif /* XSENS_H_ */
