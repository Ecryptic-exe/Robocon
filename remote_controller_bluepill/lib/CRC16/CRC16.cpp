#include <arduino.h>
#include "CRC16.h"

uint8_t* CRC16::encode(uint8_t* data, int len){
    unsigned short crc = 0xFFFF;
    if (len == 0) {
        len = 1;
    }
    while (len--) {
        crc ^= *data;
        for (int i = 0; i < 8; i++){
            if (crc & 1) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
        data++;
    }

    uint8_t temp[2] = {(uint8_t)(crc>>8), (uint8_t)crc};

    int size = sizeof(data) + sizeof(temp);
    uint8_t result[size];
    for(int i = 0; i < size; i++){
        if(i < sizeof(data)){
            result[i] = data[i];
        }
        else{
            result[i] = temp[2 - (size - i)];
        }
    }

    return result;
}

bool CRC16::decode(uint8_t* data){
    int size = sizeof(data);
    uint8_t buf[size - 2];

    for(int i = 0; i < size - 2; i++){
        buf[i] = data[i];
    }

    uint8_t* feedback = encode(buf, sizeof(buf));

    for(int i = 0; i < size; i++){
        if(data[i] != feedback[i]){
            return false;
        }
    }

    return true;
}

uint8_t* CRC16::modbus_encode(uint8_t* data, int len){
    uint8_t buf;
    uint8_t* result = encode(data, len);
    int size = sizeof(result);
    
    buf = result[size - 2];
    result[size - 2] = result[size - 1];
    result[size - 1] = buf;

    return result;
}

bool CRC16::modbus_decode(uint8_t* data){
    int size = sizeof(data);
    uint8_t buf[size - 2];

    for(int i = 0; i < size - 2; i++){
        buf[i] = data[i];
    }

    uint8_t* feedback = modbus_encode(buf, sizeof(buf));

    for(int i = 0; i < size; i++){
        if(data[i] != feedback[i]){
            return false;
        }
    }

    return true;
}
