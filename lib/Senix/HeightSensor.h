/*
 * HeightSensor.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef HEIGHTSENSOR_H_
#define HEIGHTSENSOR_H_

#include "MODSERIAL.h"
#include "utilities.h"

class HeightSensor {
private:
MODSERIAL* s;
uint32_t interval;
uint16_t transmittedPulses;
uint16_t filters;
uint8_t numberOfSamplesToBeAveraged;
void statisticsReset();
float height;
Mutex *mutex;
Mutex *busyMutex;
float senixHeightOffset;

// statisticsReset objects
Timer *statisticsTimer;
float stat_pps;     // pulses per second
uint16_t stat_eptp;   // error per 10 pulses
float stat_stdev;   // stdev measurements
uint16_t stat_stdev_count;   // amount of stdev measurements
float stat_high;    // highest distance
float stat_low;     // lowest distance
uint16_t stat_fptp;  // Filtered pulse count per 10 pulses
uint16_t stat_tptp;  // Senix pulse timeout per 10 pulses

// getAge objects
Timer ageTimer;

// thread objects
uint8_t buffer[8];

// Exceptions (objects & functions)
Timer timer;

public:
HeightSensor(MODSERIAL* s);
Semaphore *sem;
Thread *processThread;
float getHeight();
int getAge();
void thread (void const *TID);
uint16_t CRC16(uint8_t *nData, uint16_t wLength);
HeightSensorException waitForReply(uint8_t buffer[], int length);


};

#endif /* HEIGHTSENSOR_H_ */
