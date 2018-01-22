/*
 * debug.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#include "util_debug.h"
#include "mbed.h"
#include "rtos.h"

#ifdef DEBUG
Serial pc(D8, D2); //& PA_9 = D8, PA_10 = D2
Mutex pcMutex;
#endif
