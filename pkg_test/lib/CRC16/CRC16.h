#include "arduino.h"

class CRC16{
    public:
        uint8_t* encode(uint8_t* data, int len);
        bool decode(uint8_t* data);

        uint8_t* modbus_encode(uint8_t* data, int len);
        bool modbus_decode(uint8_t* data);
};