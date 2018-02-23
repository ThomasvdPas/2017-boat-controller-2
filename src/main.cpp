/*
 * main.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: boris
 */

//TODO: Add excetptions

#include <iostream>
#include <list>
#include <cmath>
#include <vector>
#include "mbed.h"
#include "rtos.h"

#include <MODSERIAL.h>

#include <Config.h>
#include <utilities.h>
#include <util_debug.h>

#include <HeightSensor.h>
#include <Xsens.h>
#include <XsensDataInterpreter.h>
#include <XsensDataParser.h>
#include <XsensErrorParser.h>

extern MODSERIAL pc;

Mutex xsensMutex;


MODSERIAL heightSensor1Serial(PA_2, PA_3,100,100); // tx, rx
MODSERIAL xsensSerial(PA_0, PA_1, 100, XSENS_READ_BUFF_SIZE); // tx, rx

HeightSensor senix(&heightSensor1Serial);
Xsens xsens(&xsensSerial);

CAN canBus(PB_8, PB_9);
CAN canMaxon(PB_8, PB_9);




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Xsens ~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void xsens_thread(){
        xsens.thread();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Senix ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void senix_thread() {
        senix.thread();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
DigitalOut led1(LED1);
Thread senixThread;
Thread xsensThread;
int main() {

      #ifdef DEBUG
        PRINT("\rBoat Height Control 2018.\n");
        #endif

        // senixThread.start(&senix_thread);
        xsensThread.start(&xsens_thread);


        return 0;
}
