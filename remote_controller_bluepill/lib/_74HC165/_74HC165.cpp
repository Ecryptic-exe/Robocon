#include "_74HC165.h"

_74HC165::_74HC165(){}

_74HC165::_74HC165(int nPL, int CP, int nCE, int data_pin, int numberBits){
    this->nPL_pin = nPL;
    this->CP_pin = CP;
    this->nCE_pin = nCE;
    this->data_pin = data_pin;

    this->numberBits = numberBits;
    setNumberBits(this->numberBits);

    digitalWrite(nCE, LOW);
}

void _74HC165::setNumberBits(int value){
    value = abs(value);
    if(value%8 == 0){
        this->numberBits = value;
    }
    else{
        int temp = (int)ceil(value/8.0f);
        this->numberBits = temp*8;
    }
    
    data = new bool[this->numberBits];
}

int _74HC165::getNumBits(){
    return numberBits;
}

bool* _74HC165::read(){
    digitalWrite(nPL_pin, LOW);
    delay(1);
    digitalWrite(nPL_pin, HIGH);

    for(int i = numberBits - 1; i >= 0; i--){
        data[i] = (bool)digitalRead(data_pin);

        digitalWrite(CP_pin, HIGH);
        delay(1);
        digitalWrite(CP_pin, LOW);
    }

    return data;
}