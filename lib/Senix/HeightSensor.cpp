/*
 * HeightSensor.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#include "HeightSensor.h"
#include "MODSERIAL.h"
#include "config.h"
#include "utilities.h"
#include "util_debug.h"

extern MODSERIAL pc;

HeightSensor::HeightSensor(MODSERIAL* senix) : _serial(senix){
        this->_serial->baud(SENIX_BAUD_RATE);
        this->_serial->rxBufferFlush();
        this->height = 0.0f;
}

void HeightSensor::thread() {
        PRINT("\rSenix thread started\n");

        double buffer;
        while(1) {

                SenixException senixException = receiveMessage(&buffer);
                if(senixException == SenixException::noException) {
                        this->height = rawHeightToMeters(buffer);
                }
        }
}

SenixException HeightSensor::receiveMessage(double* buffer) {
        Timer timer;
        timer.start();
        while(1) {
                if(USE_TIMEOUT == 1 && timer.read_ms() >= HEIGHT_SENSOR_RESPONSE_TIMEOUT) {
                        return SenixException::timeout;
                }
                if(_serial->readable()) {
                        _serial->scanf("%lf", buffer);
                        return SenixException::noException;
                }
        }
}
