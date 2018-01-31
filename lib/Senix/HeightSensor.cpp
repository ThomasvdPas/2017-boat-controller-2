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
                this->waitForReply(this->buffer);
                this->stat_pps++;
                float newHeight = this->buffer;
                PRINT("Senix Height: %f\n", newHeight);

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

HeightSensorException HeightSensor::waitForReply(uint16_t buffer) {
        timer.reset();
        timer.start();

        // if(USE_TIMEOUT == 1 && timer.read_ms() >= HEIGHT_SENSOR_RESPONSE_TIMEOUT)
        // return HeightSensorException::timeout;
        if(s->readable()) {
                s->scanf("%lf", &buffer);
                PRINT("height: %lf\n\r", buffer*0.003384*2.54);           //TODO: Make variables out of doubles
        }


        // Merge MSB and LSB of CRC from the message
        // uint16_t messageCrc = toUint16(buffer[length-1], buffer[length-2]);
        // // Calculate CRC on all fields, except the CRC fields
        // uint16_t crc = CRC16(buffer, length-2);
        // if(messageCrc != crc) {
        //         return HeightSensorException::crc;
        // }

        // return HeightSensorException::noException;

}
