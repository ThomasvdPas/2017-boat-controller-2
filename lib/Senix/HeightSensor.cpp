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

HeightSensor::HeightSensor(MODSERIAL* senix){
      this->senix = senix;
      this->senix->baud(SENIX_BAUD_RATE);
      this->senix->rxBufferFlush();
}

void HeightSensor::thread() {
      PRINT("Senix thread started");
      while(1) {
             double buffer;
             if(this->senix->readable()) {
                      this->senix->scanf("%lf", &buffer);
                      PRINT("%lf\n\r", rawHeightToMeters(buffer));
             }


      }
}
