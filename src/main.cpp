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

#include <MaxonDriver.h>
#include <MaxonMotor.h>

extern MODSERIAL pc;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CAN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
CAN canBus(PB_8, PB_9);
CanManager canBusManager(&canBus);



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Senix ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
Thread senixThread;

MODSERIAL heightSensor1Serial(PA_2, PA_3,100,100); // tx, rx
HeightSensor senix(&heightSensor1Serial);

void senix_thread() {
        senix.thread();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Xsens ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
Thread xsensThread;

MODSERIAL xsensSerial(PA_0, PA_1, 100, XSENS_READ_BUFF_SIZE); // tx, rx
Xsens xsens(&xsensSerial);

void xsens_thread(){
        xsens.thread();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~ Control loop ~~~~~~~~~~~~~~~~~~~~~~~~//


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Maxon ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
Thread maxonThread;

CAN canMaxon(PB_5, PB_6);
CanManager maxonCanManager(&canMaxon);

MaxonDriver maxonDriver1(&maxonCanManager, &canBusManager, 1);
MaxonDriver maxonDriver2(&maxonCanManager, &canBusManager, 2);



void maxon_thread() {
        maxonDriver1.thread();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
DigitalOut led1(LED1);


int main() {

      #ifdef DEBUG
        PRINT("\n\rBoat Height Control 2018.\n");
        #endif

        senixThread.start(&senix_thread);
        xsensThread.start(&xsens_thread);
        maxonThread.start(&maxon_thread);

        return 0;
}
