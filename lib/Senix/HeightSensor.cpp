/*
 * HeightSensor.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: boris
 */


#include "HeightSensor.h"

#include "MODSERIAL.h"
#include "../utilities/Config.h"
#include "util_debug.h"

MODSERIAL heightSensor1Serial(PA_2, PA_3,100,100); // tx, rx
HeightSensor heightSensor1(&heightSensor1Serial);

extern MODSERIAL pc;


HeightSensor::HeightSensor(MODSERIAL* s) : interval(0), transmittedPulses(0), filters(0), numberOfSamplesToBeAveraged(0) {
        this->s = s;
        this->sem = new Semaphore(1);
        this->s->baud(HEIGHT_SENSOR_FRONT_BUAD_RATE);
        this->s->rxBufferFlush();
        this->height = 0.0f;
        this->mutex = new Mutex();
        this->busyMutex = new Mutex();
        this->statisticsTimer = new Timer();

        this->statisticsReset();

        //TODO make this configurable
        this->senixHeightOffset = 0.22f;        // TODO: Check with current boat
}

void HeightSensor::statisticsReset() {
        this->stat_pps = 0;
        this->stat_eptp = 0;
        this->stat_stdev = 0;
        this->stat_stdev_count = 0;
        this->stat_high = 0;
        this->stat_low = 100;
        this->stat_fptp = 0;
        this->stat_tptp = 0;

        this->statisticsTimer->reset();
        this->statisticsTimer->start();
}


void HeightSensor::thread(void const *TID) {
        ageTimer.start();

        int senixcounter = 0;
        int senixerrorcounter = 0;

        float lastMeasured = 0;

        PRINT("Senix thread started \n");

        while(1) {
                this->sem->wait();
                // Thread::signal_wait(0x4);          //TODO: Ask what signal wait does.
                // The height is requested in the senixManager
                this->waitForReply(this->buffer, 7);
                this->stat_pps++;
                uint16_t data = this->buffer[3] * 0xFF + this->buffer[4];
                float newHeight = (float)(data)/115.0f;
                PRINT("Senix Height: %f\n", newHeight);
                float diff = max(height, newHeight) - min(height, newHeight);
                float diffLastMeasured = max(lastMeasured, newHeight) - min(lastMeasured, newHeight);
                if (height == 0.0f || (newHeight > 0.0f && newHeight < 100.0f && (diff < 15.0f || diffLastMeasured < 10.0f))) {
                        this->mutex->lock();
                        this->height = newHeight;
                        this->mutex->unlock();
                        this->stat_stdev += diff * diff;
                        this->stat_stdev_count++;
                        this->stat_high = max(this->height, this->stat_high);
                        this->stat_low = min(this->height, this->stat_low);
                        ageTimer.reset();
                        ageTimer.start();
                } else {
                        if (newHeight < 0.0f) {
                                this->stat_tptp++;
                        } else {
                                this->stat_fptp++;
                        }
                }

                lastMeasured = newHeight;
                this->processThread->signal_set(0x1);


                senixcounter++;
                if (senixcounter == 2000) {
                        PRINT("Senix 2000cycles, H = %.3f, errors: %i /2000, buffercount: %i \n", this->height, senixerrorcounter, this->s->rxBufferGetCount() );
                        senixcounter = 0;
                        senixerrorcounter = 0;
                }


        }
}

float HeightSensor::getHeight() {
        mutex->lock();
        float k = height/100.0f - this->senixHeightOffset;
        // PRINT("senix: %f %f %f\n", height/100.0f, this->senixHeightOffset, k);
        mutex->unlock();
        return k;
}

int HeightSensor::getAge() {
        return ageTimer.read_ms();
}

HeightSensorException HeightSensor::waitForReply(uint8_t buffer[], int length) {
        timer.reset();
        timer.start();
        // fill buffer with all 7 bytes from the message
        int count = 0;
        while(count<length) {
                if(USE_TIMEOUT == 1 && timer.read_ms() >= HEIGHT_SENSOR_RESPONSE_TIMEOUT)
                        // return HeightSensorException::timeout;
                if(s->readable()) {
                        buffer[count] = (uint8_t) s->getc();
                        count++;
                }
        }

        // Merge MSB and LSB of CRC from the message
        uint16_t messageCrc = toUint16(buffer[length-1], buffer[length-2]);
        // Calculate CRC on all fields, except the CRC fields
        uint16_t crc = CRC16(buffer, length-2);
        // if(messageCrc != crc) {
        //         return HeightSensorException::crc;
        // }

        // return HeightSensorException::noException;

}


/**
 * Calculates ModBus crc16 code
 */
uint16_t HeightSensor::CRC16(uint8_t *nData, uint16_t wLength) {
        static const uint16_t wCRCTable[] = {
                0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
                0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
                0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
                0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
                0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
                0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
                0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
                0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
                0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
                0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
                0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
                0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
                0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
                0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
                0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
                0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
                0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
                0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
                0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
                0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
                0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
                0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
                0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
                0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
                0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
                0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
                0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
                0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
                0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
                0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
                0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
                0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
        };

        uint8_t nTemp;
        uint16_t wCRCWord = 0xFFFF;

        while (wLength--) {
                nTemp = *nData++ ^ wCRCWord;
                wCRCWord >>= 8;
                wCRCWord ^= wCRCTable[nTemp];
        }
        return wCRCWord;
}
