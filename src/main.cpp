/*
 * main.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: boris
 */

#include <cmath>
#include <vector>
#include "mbed.h"

#include <MODSERIAL.h>

#include <Config.h>
#include <utilities.h>
#include <util_debug.h>

#include <HeightSensor.h>

extern MODSERIAL pc;

extern MODSERIAL heightSensor1Serial;
extern HeightSensor heightSensor1;

PIDData pidData;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Senix ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void senixRxISR1(MODSERIAL_IRQ_INFO *info){
        /*
         * This function releases the semaphore flag when the UART receives the first data message from the sensor
         */
        heightSensor1.sem->release();
}

void senixThread1(void const *TID) {
        heightSensor1.thread(TID);
}

void processHeight(void const *TID) {
        float senixHeight = 0;
        float prevSenixHeight = 0;
        while (1) {

                wait_us(100);


                if (heightSensor1.getAge() < 500) {
                        senixHeight = senixHeight * .8f + heightSensor1.getHeight() * .2f;
                }

                prevSenixHeight = senixHeight;
                pidData.h = senixHeight;
        }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
DigitalOut led1(LED1);
int main() {

      #ifdef DEBUG
        PRINT("\r\nSolar Boat Height Control 2018.\n");
        #endif

        heightSensor1Serial.baud(HEIGHT_SENSOR_FRONT_BUAD_RATE);
        heightSensor1Serial.attach(&senixRxISR1, MODSERIAL::RxIrq);

        Thread processThread(&processHeight, NULL);
        heightSensor1.processThread = &processThread;

        Thread senix1Thread(&senixThread1, NULL,osPriorityNormal, SENIX_STACK_SIZE);

        while(true) {
                led1 = !led1;
                wait(0.5);
        }
}
