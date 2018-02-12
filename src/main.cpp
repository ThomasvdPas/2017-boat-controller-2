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


// HeightSensor heightSensor1(&heightSensor1Serial);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Xsens ~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void xsens_request_baud(){
        if(xsensSerial.writeable()) {
                uint8_t req[5] = {0xFA, 0xFF, 0x18, 0x00, 0xE9};
                // xsensMutex.lock();
                for (int i=0; i<5; i++) {
                        xsensSerial.putc(req[i]);
                }
                // xsensMutex.unlock();
        }
}

void xsens_config_mode(){
        if(xsensSerial.writeable()) {
                uint8_t req[5] = {0xFA, 0xFF, 0x30, 0x00, 0xD1};
                // xsensMutex.lock();
                for (int i=0; i<5; i++) {
                        xsensSerial.putc(req[i]);
                }
                // xsensMutex.unlock();
        }
}

void xsens_measurement_mode(){
        if(xsensSerial.writeable()) {
                uint8_t req[5] = {0xFA, 0xFF, 0x10, 0x00, 0xF1};
                // xsensMutex.lock();
                for (int i=0; i<5; i++) {
                        xsensSerial.putc(req[i]);
                }
                // xsensMutex.unlock();
        }
}

void xsens_temp_mode(){
        int length = 7;
        if(xsensSerial.writeable()) {
                uint8_t req[length] = {0xFA, 0xFF, 0xD0, 0x02, 0x00, 0x01, 0x2E};
                // xsensMutex.lock();
                for (int i=0; i<length; i++) {
                        xsensSerial.putc(req[i]);
                }
                // xsensMutex.unlock();
        }
}

void xsens_pos_mode(){
        int length = 7;
        if(xsensSerial.writeable()) {
                uint8_t req[length] = {0xFA, 0xFF, 0xD0, 0x02, 0x00, 0x10, 0x1F};
                // xsensMutex.lock();
                for (int i=0; i<length; i++) {
                        xsensSerial.putc(req[i]);
                }
                // xsensMutex.unlock();
        }
}


void xsens_thread(){
        // xsens.thread();          //TODO: Write this class into the thread
        static bool preambleFound = false;
        uint8_t buffer[100];
        int bufferCount = 0;
        xsensSerial.baud(115200);
        PRINT("Xsens thread started");


        // xsens_config_mode();
        // xsens_request_baud();
        // // xsens_pos_mode();
        // xsens_measurement_mode();
        while(1) {

                if(xsensSerial.readable()) {
                        uint8_t res = (uint8_t) xsensSerial.getc();
                        if (res == XSENS_PREAMBLE) {
                                preambleFound = true;
                        }
                        if (preambleFound) {
                                if (res == XSENS_PREAMBLE && buffer[0] == XSENS_PREAMBLE)
                                {
                                        for(int i=0; i<bufferCount; ++i)
                                        {
                                                PRINT("%02x", buffer[i]);
                                                buffer[i] = 0;
                                        }
                                        PRINT("\r\n");
                                        bufferCount = 0;
                                        preambleFound = false;
                                }
                                else{
                                        buffer[bufferCount] = res;
                                        bufferCount++;
                                }
                        }
                }
        }
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
        PRINT("Boat Height Control 2018.\n");
        #endif

        // senixThread.start(&senix_thread);
        xsensThread.start(&xsens_thread);


        return 0;
}
