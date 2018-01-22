/*
 * debug.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "mbed.h"

#define USE_TIMEOUT 1
#define DEBUG 1
//#define LOGGER 1


#ifdef DEBUG
	#define PRINT(...) pcMutex.lock();\
	pc.baud(57600); \
	pc.printf(__VA_ARGS__);\
	pc.printf("\r");\
	pcMutex.unlock();
#else
	#define PRINT(...)
#endif

#endif /* DEBUG_H_ */
