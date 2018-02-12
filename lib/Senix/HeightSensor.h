/*
 * HeightSensor.h
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */

#ifndef HEIGHTSENSOR_H_
#define HEIGHTSENSOR_H_

#include "MODSERIAL.h"

class HeightSensor {
private:
      MODSERIAL* senix;
public:
      HeightSensor(MODSERIAL* senix);
      void thread();
};

#endif /* HEIGHTSENSOR_H_ */
