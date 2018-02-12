/*
 * debug.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "mbed.h"

#define DEBUG 1
#define USE_TIMEOUT 1

#ifdef DEBUG
	#define PRINT(...) pc.baud(57600); \
	pc.printf(__VA_ARGS__);
#else
	#define PRINT(...)
#endif

#endif /* DEBUG_H_ */

/*TODO: was:
#define PRINT(...) pc.baud(57600); \
pc.printf("\r");\
pc.printf(__VA_ARGS__);\
pc.printf("\n");
*/
