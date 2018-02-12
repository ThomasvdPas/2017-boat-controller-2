/*
 * utilities.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>
#include <vector>
#include "util_debug.h"
#include "bit_operations.h"
#include "exceptions.h"
#include "MODSERIAL.h"

#define PI         3.14159265
#define ANGLE_TO_QC      39614
#define HOMING_ANGLE     15.5
#define MIN_ANGLE      -12
#define NEUTRAL_WING_POSITION_TRESHOLD 1000

float toRadians(float degrees);
int32_t toQc(float angle);
float rawHeightToMeters(uint16_t rawHeight);


float toFloat(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
uint16_t toUint16(uint8_t a, uint8_t b);
uint32_t toUint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
uint32_t toUint32(uint16_t a, uint16_t b);

void modserialPutArray(MODSERIAL* s, char array[], int length);
void modserialPutArray(MODSERIAL* s, uint8_t array[], int length);

int32_t getNeutralWingQc(float neutralWingAngle);
int32_t absoluteAngleToQc(float absoluteAngle);

bool in_array(const string &value, const std::vector<string> &array);

typedef union {
								float f[2];
								char c[8];
} two_floats_to_8_bytes_t;

typedef union {
								float f;
								char c[4];
} float_to_4_bytes_t;

typedef union {
								int32_t i;
								char c[4];
} int32_to_4_bytes_t;

typedef union {
								uint32_t i;
								char c[4];
} uint32_to_4_bytes_t;

typedef union {
								uint16_t i;
								char c[2];
} uint16_to_2_bytes_t;

typedef union {
								struct {
																uint16_t year;
																uint8_t month;
																uint8_t day;
																uint8_t hour;
																uint8_t minute;
																uint8_t second;
																uint8_t flags;
								};
								char c[8];
} time_to_8_bytes_t;

#endif /* UTILITIES_H_ */
