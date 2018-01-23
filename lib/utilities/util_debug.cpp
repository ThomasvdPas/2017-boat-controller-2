/*
 * debug.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#include "util_debug.h"
#include "mbed.h"
#include "rtos.h"
#include "MODSERIAL.h"

#ifdef DEBUG
MODSERIAL pc(PA_9, PA_10,100,100); //& PA_9 = D8, PA_10 = D2
Mutex pcMutex;
#endif
