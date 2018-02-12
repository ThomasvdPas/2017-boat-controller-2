/*
 * Xsens.cpp

 *
 *  Created on: 13 feb. 2014
 *      Author: boris
 */
#include <stdlib.h>
#include <stdio.h>
#include "rtos.h"
#include "utilities.h"
#include "exceptions.h"
#include "Xsens.h"
#include "XsensDataParser.h"
#include "XsensErrorParser.h"
#include "MODSERIAL.h"
#include "config.h"
#include "util_debug.h"

#ifdef DEBUG
extern Serial pc;
extern Mutex pcMutex;
#endif


Xsens::Xsens(MODSERIAL* xsens) : _serial(xsens){
        this->_serial->baud(XSENS_BAUD_RATE);
        this->_serial->rxBufferFlush();
        this->interpreter = new XsensDataInterpreter(&(this->xsensData));
}

// void Xsens::printState() {
//         this->interpreter->printState();
// }

void Xsens::thread() {
        PRINT("Xsens thread started\n\r");

        XsensDataParser xdp(&(this->xsensData));
        XsensException xe;
        XsensErrorParser xep(&xe);

        uint8_t bufferLength = 0;
        uint8_t buffer[XSENS_MESSAGE_BUFFER_LENGTH] = {0};
        int bufferSize = XSENS_MESSAGE_BUFFER_LENGTH;


        while(1) {
                XsensException xsensException = receiveMessage(buffer, &bufferLength, bufferSize);
                PRINT("\r\n%d", xsensException);
                if(xsensException == XsensException::noException) {
                        if(buffer[XSENS_MESSAGE_POSITION_MID] == XSENS_MESSAGE_MTDATA2) {
                                xdp.parse(buffer, bufferLength);
                        }
                        else if(buffer[XSENS_MESSAGE_POSITION_MID] == XSENS_MESSAGE_ERROR) {
                                xep.parse(buffer, bufferLength);
                                PRINT("%i", xe);
                        }else{PRINT("What to do next?");}
                }
        }
}



XsensException Xsens::receiveMessage(uint8_t buffer[], uint8_t* bufferCount, uint16_t bufferSize) {
        *bufferCount = 0;
        uint8_t headerCsLength = 5;
        uint16_t length = 0;
        uint8_t sum = 0;

        bool preambleFound = 0;

        Timer timer;
        timer.start();
        // make sure the length field gets read, and terminate when all message bytes were read
        while(1) {

                if(USE_TIMEOUT == 1 && timer.read_ms() >= XSENS_DEFAULT_TIMEOUT) {
                        return XsensException::timeout;
                }
                if(_serial->readable()) {
                        uint8_t c = (uint8_t) _serial->getc();
                        //PRINT("%02x \r\n", c & 0xFF);
                        if(preambleFound == false && c ==XSENS_MESSAGE_PREAMBLE) {
                                preambleFound = true;
                        }

                        if(preambleFound) {
                                // check to prevent memory overflow
                                if(length > XSENS_MESSAGE_BUFFER_LENGTH) {
                                        return XsensException::messageTooLarge;
                                }
                                // length if not extended
                                if(*bufferCount == 3 && c != XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
                                        length = c;
                                }
                                // length if extended
                                if(*bufferCount == 4 && buffer[3] == XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
                                        length = c << 8;
                                        headerCsLength = 6;
                                }
                                if(*bufferCount == 5 && buffer[3] == XSENS_MESSAGE_USE_EXTENDED_LENGTH) {
                                        length |= (uint16_t) c;
                                }

                                if (*bufferCount > bufferSize) {
                                        PRINT("\r\nXsens buffer exceeded (required: %d, actual: %d), abort.\r\n", length, bufferSize);
                                        // TODO voordat je returnt eerst de buffer leeg lezen
                                        return XsensException::messageTooLarge;
                                }
                                buffer[*bufferCount] = c;
                                // skip preamble in checksum calculation
                                if(*bufferCount > 0) {
                                        sum += c;
                                }
                                (*bufferCount)++;



                                // wait untill message is fully buffered
                                if(*bufferCount > 3) {
                                        if(buffer[3] == XSENS_MESSAGE_USE_EXTENDED_LENGTH && *bufferCount > 6 && (*bufferCount - headerCsLength) == length) {
                                                break;
                                        } else if(*bufferCount - headerCsLength == length) {
                                                break;
                                        }
                                }
                        }
                }
        }



        //PRINT("\n");
        // Check if header fields have correct values
        if(   buffer[XSENS_MESSAGE_POSITION_PREAMBLE] != XSENS_MESSAGE_PREAMBLE ||
              buffer[XSENS_MESSAGE_POSITION_BID] != XSENS_MESSAGE_BID) {
                return XsensException::wrongMessageHeader;
        }

        // Check checksum
        if(sum != 0) {
                PRINT("\r\n%02x\r\n", sum);
                return XsensException::checksum;
        }

        return XsensException::noException;
}
