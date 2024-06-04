#ifndef _74HC165_H
#define _74HC165_H

#include <Arduino.h>

class _74HC165{
    private:
        int nPL_pin;
        int CP_pin;
        int nCE_pin;
        int data_pin;

        int numberBits = 8;

        bool* data;

    public:
        _74HC165();
        _74HC165(int npl, int cp, int nce, int data, int numBits);

        void setNumberBits(int numberBits);
        int getNumBits();
        bool* read();
};

#endif