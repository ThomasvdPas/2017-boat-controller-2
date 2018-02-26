/*
 * exceptions.h
 *
 *  Created on: Mar 25, 2014
 *      Author: boris
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

enum class GeneralException : uint8_t {
	watchdogTimerReset
};

enum class XsensException : uint8_t {
	noException,
	timeout,
	wrongMessageHeader,
	checksum,
	messageTooLarge,
	errorMessageReceived,
	xsensPeriodSentInvalidRange,
	xsensMessageSentInvalid,
	xsensTimerOverflow,
	xsensBaudRateInvalidRange,
	xsensParameterSentInvalid,
	messageBuffered,
};

enum class SenixException : uint8_t {
	noException,
	timeout,
	sensor,
	crc,
	wrongMessage,
	setFilters,
	setNumberOfSamplesToBeAveraged,
	setInterval,
	setTransmittedPulses
};

enum class MaxonMotorException : uint8_t {
	noException,
	timeout,
	setHomingOperation,
	setCurrentTreshold,
	setHomingSpeed,
	setHomingOffset,
	setHomePosition,
	setHomingMethod,
	setShutDown,
	setSwitchOn,
	setStartHoming,
	setPositionProfileOperation,
	setPositionOperation,
	setTarget,
	positionModeSettingValue,
	maskReadyToSwitchOn,
	maskOperationEnable,
	maskHomeReached,
	maskFault,
	setResetFault,
	maxonSpecificException,
	targetSetTooHigh,
	targetSetTooLow,
	setAcceleration,
	setDeceleration,
	setMaxFollowingError,
	setVelocity,
	neutralReached
};

enum class WriteCanException : uint8_t {
	noException,
	timeout
};


#endif /* EXCEPTIONS_H_ */
