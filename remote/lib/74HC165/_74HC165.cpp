#include "_74HC165.h"

_74HC165::_74HC165(PinName pin1, PinName pin2, PinName pin3, int numBits) : dataPin(pin1), clockPin(pin2), latchPin(pin3){
    setNumBits(numBits);
}

void _74HC165::setNumBits(int value){
    value = abs(value);
    if(value%8 == 0){
        this->numBits = value;
    }
    else{
        int temp = (int)ceil(value/8.0f);
        this->numBits = temp*8;
    }
    
    buff = new bool[this->numBits];
}

bool* _74HC165::getData(){
    latchPin = 0;
    __NOP();
    //wait_us(1);
    latchPin = 1;

    for(int i = numBits - 1; i >= 0; i--){
        buff[i] = (uint8_t)dataPin.read();

        clockPin = 1;
        // wait_us(1);
        __NOP(); 
        clockPin = 0;
        // printf("buff[%d]: %d\n", i, buff[i]);
    }

    return buff;
}

int _74HC165::getNumBits(){
    return numBits;
}