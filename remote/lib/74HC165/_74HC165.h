#ifndef _74HC165_H
#define _74HC165_H

#include "mbed.h"

class _74HC165
{
    private:
        DigitalIn dataPin;
        DigitalOut clockPin;
        DigitalOut latchPin;

        int numBits = 8;
        bool* buff;

    public:
        /** Creates 74HC165 object with specific content.
         *
         *  @param pin1     Data Pin
         *  @param pin2     Clock Pin
         *  @param pin3     Latch Pin
         *  @param numBits  The amount of data (bit)
         */
        _74HC165(PinName pin1, PinName pin2, PinName pin3, int numBits);

        /** Set the number of data of 74HC165(s)
         *
         *  @param value    the number of data (bit)
         */
        void setNumBits(int value);

        /* Get the values of 74HC165*/
        bool* getData();
        int getNumBits();
};

#endif