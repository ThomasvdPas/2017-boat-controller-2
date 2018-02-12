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
// void xsens_request_baud(){
//         if(xsensSerial.writeable()) {
//                 uint8_t req[5] = {0xFA, 0xFF, 0x18, 0x00, 0xE9};
//                 // xsensMutex.lock();
//                 for (int i=0; i<5; i++) {
//                         xsensSerial.putc(req[i]);
//                 }
//                 // xsensMutex.unlock();
//         }
// }
//
// void xsens_config_mode(){
//         if(xsensSerial.writeable()) {
//                 uint8_t req[5] = {0xFA, 0xFF, 0x30, 0x00, 0xD1};
//                 // xsensMutex.lock();
//                 for (int i=0; i<5; i++) {
//                         xsensSerial.putc(req[i]);
//                 }
//                 // xsensMutex.unlock();
//         }
// }
//
// void xsens_measurement_mode(){
//         if(xsensSerial.writeable()) {
//                 uint8_t req[5] = {0xFA, 0xFF, 0x10, 0x00, 0xF1};
//                 // xsensMutex.lock();
//                 for (int i=0; i<5; i++) {
//                         xsensSerial.putc(req[i]);
//                 }
//                 // xsensMutex.unlock();
//         }
// }
//
// void xsens_temp_mode(){
//         int length = 7;
//         if(xsensSerial.writeable()) {
//                 uint8_t req[length] = {0xFA, 0xFF, 0xD0, 0x02, 0x00, 0x01, 0x2E};
//                 // xsensMutex.lock();
//                 for (int i=0; i<length; i++) {
//                         xsensSerial.putc(req[i]);
//                 }
//                 // xsensMutex.unlock();
//         }
// }
//
// void xsens_pos_mode(){
//         int length = 7;
//         if(xsensSerial.writeable()) {
//                 uint8_t req[length] = {0xFA, 0xFF, 0xD0, 0x02, 0x00, 0x10, 0x1F};
//                 // xsensMutex.lock();
//                 for (int i=0; i<length; i++) {
//                         xsensSerial.putc(req[i]);
//                 }
//                 // xsensMutex.unlock();
//         }
// }
//
// uint8_t * request_data(){
//         static bool preambleFound = false;
//
//         static uint8_t buffer[XSENS_MESSAGE_BUFFER_LENGTH] = {0};
//         static uint16_t length = 0;
//         static uint8_t headerCsLength = 4;
//
//         int bufferCount = 0;
//
//         while(1) {
//                 uint8_t res = (uint8_t) xsensSerial.getc();
//
//                 if (res == XSENS_PREAMBLE) {
//                         if(preambleFound) {
//                                 PRINT("\n%02x", bufferCount);
//                                 return buffer;
//                         }
//                         else{preambleFound = true;}
//                 }
//
//                 if (preambleFound) {
//
//                         // check to prevent memory overflow
//                         if (length > XSENS_MESSAGE_BUFFER_LENGTH) {
//                                 PRINT("messageTooLarge");
//                         }
//                         // length if not extended
//                         if (bufferCount == 2 && res != XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
//                                 PRINT("\n%02x\r", res)
//                                 length = res;
//                         }
//
//                         // length if extended
//                         if (bufferCount == 3 && buffer[2] == XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
//                                 length = res << 7;
//                                 headerCsLength = 5;
//                         }
//                         if (bufferCount == 4 && buffer[2] == XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
//                                 length |= (uint16_t) res;
//                         }
//
//                         // wait untill message is fully buffered
//                         if (bufferCount > 3) {
//                                 if (length == bufferCount + headerCsLength ) {
//                                         // PRINT("%02x", res);
//                                 }
//                         }
//                         buffer[bufferCount] = res;
//                         bufferCount++;
//                 }
//         }
// }
//
// void receiveMessage(uint8_t buffer[], uint8_t* bufferCount, uint16_t bufferSize) {
//         *bufferCount = 0;
//         uint8_t headerCsLength = 5;
//         uint16_t length = 0;
//         uint8_t sum = 0;
//         uint8_t strlen = 0;
//
//         bool preambleFound = 0;
//
//         while(1) {
//                 if(xsensSerial.readable()) {
//                         uint8_t c = (uint8_t) xsensSerial.getc();
//                         //PRINT("%02x \r\n", c & 0xFF);
//                         if(preambleFound == false && c ==XSENS_MESSAGE_PREAMBLE) {
//                                 preambleFound = true;
//                         }
//
//                         if(preambleFound) {
//                                 // check to prevent memory overflow
//                                 if(length > XSENS_MESSAGE_BUFFER_LENGTH) {
//                                 }
//                                 // length if not extended
//                                 if(*bufferCount == 3 && c != XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
//                                         PRINT("\n\r %02x \n\r", c);
//                                         length = c;
//                                 }
//                                 // length if extended
//                                 if(*bufferCount == 4 && buffer[3] == XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
//                                         length = c << 8;
//                                         headerCsLength = 6;
//                                 }
//                                 if(*bufferCount == 5 && buffer[3] == XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
//                                         length |= (uint16_t) c;
//                                 }
//
//                                 if (strlen > bufferSize) {
//                                         PRINT("\r\nXsens buffer exceeded (required: %d, actual: %d), abort.\r\n", length, bufferSize);
//                                         // TODO voordat je returnt eerst de buffer leeg lezen
//                                 }
//                                 buffer[*bufferCount] = c;
//                                 // skip preamble in checksum calculation
//                                 if(*bufferCount > 0) {
//                                         sum += c;
//                                 }
//                                 (*bufferCount)++;
//                                 strlen++;
//
//
//
//                                 // wait untill message is fully buffered
//                                 if(*bufferCount > 3) {
//                                         if(buffer[3] == XSENS_MESSAGE_USE_EXTENDED_LENGTH && *bufferCount > 6 && (*bufferCount - headerCsLength) == length) {
//                                                 break;
//                                         } else if(*bufferCount - headerCsLength == length) {
//                                                 break;
//                                         }
//                                 }
//                         }
//                 }
//         }
//         if(sum != 0) {
//                 PRINT("%02x", sum);
//         }
// }
//
//

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
        PRINT("Boat Height Control 2018.\n");
        #endif

        // senixThread.start(&senix_thread);
        xsensThread.start(&xsens_thread);


        return 0;
}
