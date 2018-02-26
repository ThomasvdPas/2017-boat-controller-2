/*
 * Config.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define XSENS_BAUD_RATE       460800
#define SENIX_BAUD_RATE   38400

// Buffer Xsens
#define XSENS_HEADER_SHORT     4 // without ext length field
#define XSENS_HEADER_LONG      6 // with ext length field
#define XSENS_HEADER_SHORT_CS     (XSENS_HEADER_SHORT+1) // including CS
#define XSENS_HEADER_LONG_CS    (XSENS_HEADER_LONG+1)
#define XSENS_DAT_LEN_TIME     12
#define XSENS_DAT_LEN_ROTATION    36
#define XSENS_DAT_LEN_ACCELERATION   12
#define XSENS_DAT_LEN_LATLON     8
#define XSENS_DAT_LEN_VELOCITY    12
#define XSENS_DAT_LEN_STATUSWORD    4
#define XSENS_MESSAGE_TOTAL_LEN   (XSENS_HEADER_LONG_CS + XSENS_DAT_LEN_TIME + XSENS_DAT_LEN_ROTATION + XSENS_DAT_LEN_ACCELERATION + XSENS_DAT_LEN_LATLON + XSENS_DAT_LEN_VELOCITY + XSENS_DAT_LEN_STATUSWORD)

#define XSENS_READ_BUFF_SIZE   XSENS_MESSAGE_TOTAL_LEN * 16
#define XSENS_MESSAGE_BUFFER_LENGTH  1024

#define XSENS_DEFAULT_TIMEOUT    3000
#define XSENS_WAKE_UP_TIMEOUT   3000
#define XSENS_RESET_TIMEOUT    2000
#define XSENS_STACK_SIZE     2024
#define XSENS_MAXDATALEN     8192
#define XSENS_MESSAGE_BUFFER_LENGTH  1024
#define XSENS_MSG_MTDATA2_ID    0x36
#define XSENS_MSG_ERROR_ID    0x42
#define XSENS_PREAMBLE     0xFA
#define XSENS_BID       0xFF

// xsens message
#define XSENS_MESSAGE_PREAMBLE    0xFA
#define XSENS_MESSAGE_BID     0xFF
#define XSENS_MESSAGE_MTDATA2    0x36
#define XSENS_MESSAGE_ERROR     0x42
#define XSENS_MESSAGE_USE_EXTENDED_LENGTH 0xFF

// xsens message positions
#define XSENS_MESSAGE_POSITION_PREAMBLE 0
#define XSENS_MESSAGE_POSITION_BID  1
#define XSENS_MESSAGE_POSITION_MID  2

// Senix
#define SENIX_STACK_SIZE   1024
#define SMALL_STACK_SIZE   512
#define HEIGHT_SENSOR_RESPONSE_TIMEOUT    50

#define HEIGHTSENSOR_DISTANCE_TO_WATER_FRONT 0.4
#define TRESHOLD_ZERO_ABOVE_WATER    0.05

#define FILTER_OFFSET_D_DIFF_PERCENTAGE   1

// CAN
//Isr timers
#define REPORT_TO_CAN_WAIT						0.5
#define REPORT_STATUS_WAIT						0.5
#define STEER_ANGLE_RATIO						0.2582f

#define REPORT_TO_CAN_LAT_LON_ID					0x220
#define REPORT_TO_CAN_ALT_VELX_ID					0x221
#define REPORT_TO_CAN_VELY_VELZ_ID					0x222
#define REPORT_TO_CAN_ACCX_ACCY_ID					0x223
#define REPORT_TO_CAN_ACCZ_PITCH_ID					0x224
#define REPORT_TO_CAN_ROLL_YAW_ID					0x225
#define REPORT_TO_CAN_ROTX_ROTY_ID					0x226
#define REPORT_TO_CAN_ROTZ_ID						0x227
#define REPORT_TO_CAN_TIME_ID						0x228
#define REPORT_TO_CAN_HEIGHT_ID						0x229
#define REPORT_TO_CAN_VELOCITY_BOAT_ID				0x22A
#define REPORT_TO_CAN_VELO_ID						0X290
#define REPORT_TO_CAN_CONTROLSYSTEM_OUTPUT_ID		0x22B
#define WING_CONTROL_ERROR_CAN_ID					0x22C
#define REPORT_TO_CAN_DESIRED_HEIGHT_AUTO_VALUE_ID	0x22D
#define REPORT_TO_CAN_CURRENT_ANGLE_FRONT_ID		0x22E
#define REPORT_TO_CAN_CURRENT_ANGLE_REAR_ID			0x232
#define REPORT_TO_CAN_K_H_OUTPUT_ID					0x22F
#define REPORT_TO_CAN_K_THETA_OUTPUT_ID				0x230
#define REPORT_TO_CAN_K_Q_OUTPUT_ID					0x231
#define REPORT_TO_CAN_BASEGAIN_OUTPUT_ID			0x233

#define REPORT_TO_CAN_STATUS_FRONT_ID				0x279
#define REPORT_TO_CAN_CONTROL_SYSTEM_HEIGHT_ID 0x247
#define REPORT_TO_CAN_CONTROL_SYSTEM_HEIGHT_REAR_ID 0x248
#define REPORT_TO_CAN_STATUS_REAR_ID				0X27A

#define REPORT_TO_CAN_SENIX							0x500
#define REPORT_TO_CAN_SENIX_STATISTICS				0x501 // 0x501+0 for node 1, and 0x501+1 for node 2
#define REPORT_TO_CAN_MAXON							0x503 // 0x503+0 for node 1, and 0x503+1 for node 2
#define REPORT_TO_CAN_PID_SENSORS					0x505
#define REPORT_TO_CAN_PID_INPUTS					0x506
#define REPORT_TO_CAN_PID_GAINS						0x507
#define REPORT_TO_CAN_PID_OUT						0x508
#define UPDATE_FROM_CAN								0x509
#define REPORT_TO_CAN_STEERANGLE					0x50A
#define REPORT_TO_CAN_SENSORPANIC					0x50B
#define CAN_BUS_DESIRED_HEIGHT_ID 				0x240

#define REQUEST_DESIRED_HEIGHT_ID				0x242
#define CAN_WRITE_DEFAULT_TIMEOUT_US			1500


#define DESIRED_HEIGHT_AUTO      0xFF

#define MS_TO_KMH        3.6
#define DESIRED_HEIGHT_AUTO_VELOCITY_MIN  6.318840579710144927536231884058
#define DESIRED_HEIGHT_AUTO_VELOCITY_MAX  10.956521739130434782608695652174
#define DESIRED_HEIGHT_AUTO_CONSTANT   0.129375
#define DESIRED_HEIGHT_MAX      0.6


#define PID_P_VELOCITY_FACTOR   0.007 //With this value, the P factor is halved at top speed
#define PID_D_VELOCITY_FACTOR   0.075

#define TOTAL_ACCEPTED_CAN_IDS     13

// MAXON calibration
#define MAXON_NEUTRAL_POSITION	 -360000
#define MAXON_MAX_NEGATIVE_ANGLE -620000
#define MAXON_MAX_POSITIVE_ANGLE 0
#define MAXON_NEXT_PITCH_MINIMAL_DISTANCE 500
#define MAXON_MAXRAD			 0.3371f
#define MAXON_MINRAD			 -0.2288f
#define MAXON_ZEROLIFT_RAD		 -0.04f
#define MAXON_TOTALRAD			 (MAXON_MAXRAD-MAXON_MINRAD)
#define MAXON_POSITION_MARGIN	 50000


//Statusbit layout
#define StatusGPS 		0
#define StatusHoming 	1
#define StatusMaxon 	2
#define StatusKalman 	3
#define StatusAutoMode	4
#define StatusRoll		5
#define StatusGPSLoss 	6

enum class CanErrorCategory : uint8_t {
								general,
								xsens,
								heightsensor1,
								heightsensor2,
								maxonmotor1,
								maxonmotor2
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
