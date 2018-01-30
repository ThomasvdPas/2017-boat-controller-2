/*
 * Config.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "HeightSensor.h"

#define XSENS_BAUD_RATE							460800
#define HEIGHT_SENSOR_FRONT_BUAD_RATE			57600

#define SENIX_STACK_SIZE			1024
#define SMALL_STACK_SIZE			512
#define HEIGHT_SENSOR_RESPONSE_TIMEOUT    50

#define HEIGHTSENSOR_DISTANCE_TO_WATER_FRONT	0.4
#define TRESHOLD_ZERO_ABOVE_WATER				0.05

#define FILTER_OFFSET_D_DIFF_PERCENTAGE			1

#define CAN_BUS_DESIRED_HEIGHT_ID 				0x240
#define REQUEST_DESIRED_HEIGHT_ID				0x242

#define CAN_HEIGHT_SENSOR_NO_OF_SAMPLES_TO_BE_AVERAGED_ID	0x266

#define CAN_WRITE_DEFAULT_TIMEOUT_US			1500

#define DESIRED_HEIGHT_AUTO						0xFF

#define MS_TO_KMH								3.6
#define DESIRED_HEIGHT_AUTO_VELOCITY_MIN		6.318840579710144927536231884058
#define DESIRED_HEIGHT_AUTO_VELOCITY_MAX		10.956521739130434782608695652174
#define DESIRED_HEIGHT_AUTO_CONSTANT			0.129375
#define DESIRED_HEIGHT_MAX						0.6


#define PID_P_VELOCITY_FACTOR 		0.007 //With this value, the P factor is halved at top speed
#define PID_D_VELOCITY_FACTOR			0.075

#define TOTAL_ACCEPTED_CAN_IDS					13


enum class CanErrorCategory : uint8_t {
	general,
	xsens,
	heightsensor1,
	heightsensor2,
	maxonmotor
};

typedef struct {
	float P;
	float I;
	float D;
	float DD;
} process_values_t;


typedef struct {
        float K_q = 0.019f; // pitch rate gain TODO make this gain configurable
        float K_h = .19f; // height gain TODO make this gain configurable
        float K_theta = 1.5f; // pitch gain TODO make this gain configurable
        float h_ref = 10.0f; // The height reference  (m)
        float h = 9.5f; // The actual height (m)
        float q = 0.1f; // The actual pitch rate (rad/s)
        float theta; // actual pitch
        float alpha_eq = 0.0154f; // Angle of attack equilibrium point (rad)
        float K_p = 0.1f; // Roll rate gain
        float K_phi = 0.55f; // Roll angle gain
        float phi_ref = 0.0f; // Desired roll angle (rad)
        float phi = 0.0f; // Measured roll angle (rad)
        float p = 0.0f; // Measured roll rate (rad/s)
        float v = 0.0f; // Measured boat velocity (m/s)
        float r = 0.0f; // Measured yaw rate (rad/s)
} PIDData;


#endif /* CONFIG_H_ */
