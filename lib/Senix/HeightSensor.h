/*
 * HeightSensor.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef HEIGHTSENSOR_H_
#define HEIGHTSENSOR_H_

#include "MODSERIAL.h"
#include "exceptions.h"

class HeightSensor {
private:
      MODSERIAL* _serial;
      float height;

public:
      HeightSensor(MODSERIAL*);
      void thread();

      SenixException receiveMessage(double* buffer);
};

#endif /* HEIGHTSENSOR_H_ */
