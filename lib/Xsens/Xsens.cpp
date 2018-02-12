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

#ifdef DEBUG
extern Serial pc;
extern Mutex pcMutex;
#endif


Xsens::Xsens(MODSERIAL* xsens) : _serial(xsens){
        this->_serial->baud(XSENS_BAUD_RATE);
        this->_serial->rxBufferFlush();
        this->interpreter = new XsensDataInterpreter(&(this->xsensData));
}

void Xsens::printState() {
        this->interpreter->printState();
}

void Xsens::thread() {
        XsensDataParser xdp(&(this->xsensData));
        XsensException xe;
        XsensErrorParser xep(&xe);

        uint8_t bufferLength = 0;

        int xsenscounter = 0;
        int Xsenserrorcounter = 0;
        int Vcounter = 0;
        int bufferSize = XSENS_MESSAGE_BUFFER_LENGTH;

        Timer XsensTimer;

        PRINT("Xsens_thread started\n");
        while(1) {
                //Wait for the Rx interrupt to clear the semaphore
                if (xsenscounter == 0) {
                        XsensTimer.start();
                }

                uint8_t buffer[XSENS_MESSAGE_BUFFER_LENGTH] = {0};

                XsensException xsensException = receiveMessage(buffer, &bufferLength, bufferSize);
                // PRINT("Xsens Exception: %d", xsensException);
                if(xsensException == XsensException::noException) {
                        if(buffer[XSENS_MESSAGE_POSITION_MID] == XSENS_MESSAGE_MTDATA2) {
                                xdp.parse(buffer, bufferLength);

                                // this->xsensData_mutex->lock();
                                printState();
                                // this->xsensData_mutex->unlock();
                                // //checkGPS(); //check if there is GPSlock.
                                // this->processThread->signal_set(0x1);



                        } else if(buffer[XSENS_MESSAGE_POSITION_MID] == XSENS_MESSAGE_ERROR) {
                                Xsenserrorcounter += 0;
                                xep.parse(buffer, bufferLength);
                                PRINT("%i", xe);
                                //writeCanError(CanErrorCategory::xsens, (uint8_t) xe); //TODO write can error
                                Xsenserrorcounter++;
                        }


                }
                xsenscounter++;

                if (xsenscounter == 4000) {
                        XsensTimer.stop();
                        PRINT("Xsens 4000 cycles dt: %f sec, %i /4000 errors, buffercount %i \n",
                              XsensTimer.read(), Xsenserrorcounter, this->_serial->rxBufferGetCount());
                        xsenscounter = 0;
                        Xsenserrorcounter = 0;
                        XsensTimer.reset();
                }

        }

}



XsensException Xsens::receiveMessage(uint8_t buffer[], uint8_t* bufferCount, uint16_t bufferSize) {
	*bufferCount = 0;
	uint8_t headerCsLength = 5;
	uint16_t length = 0;
	uint8_t sum = 0;
	uint8_t strlen = 0;

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

			if(preambleFound == false && c ==XSENS_MESSAGE_PREAMBLE) {
                        // PRINT("preamble found: %02x \r\n", c);
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

				if (strlen > bufferSize) {
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
				strlen++;



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
	if(	buffer[XSENS_MESSAGE_POSITION_PREAMBLE] != XSENS_MESSAGE_PREAMBLE ||
		buffer[XSENS_MESSAGE_POSITION_BID] != XSENS_MESSAGE_BID) {
		return XsensException::wrongMessageHeader;
	}

	// Check checksum
	if(sum != 0) {
		return XsensException::checksum;
	}

	return XsensException::noException;
}
