#ifndef REMOTE_H
#define REMOTE_H

#include "mbed.h"
#include "communication_format.h"
#include "_74HC165.h"
#include "map.hpp"

class remote{
    private:
        remote_msg msg;
        BufferedSerial* serial;

        _74HC165* hc165;

        int x1_pin, y1_pin, x2_pin, y2_pin;
        int home_pin;
        int battery_level_pin;

        int x_max = 4084;
        int x_mid = 2067;
        int x_min = 3;

        int y_max = 4083;
        int y_mid = 2060;
        int y_min = 3;

        int w_max = 4089;
        int w_mid = 2050;
        int w_min = 4;

        int allow_offset = 100;

        int8_t x1, y1, x2, y2;
        int8_t op1, op2, op3, op4, op5;
        bool up_flag, down_flag, Left_flag, Right_flag, A_flag, B_flag, X_flag, Y_flag, L1, L2, R1, R2, LjoystickSW, RjoystickSW, ESTOP, Reset_button, Home;
        bool* data;

        //remote controller status
        float volt_min = 3.7f;
        float volt_max = 4.2f;
        float volt = 0.0f;
        float volt_percentage = 0.0f;

    public:
        void setJoystickPins(int x1_pin, int y1_pin, int x2_pin, int y2_name);
        void setHomePin(int home_pin);
        void setBattery_Pin(int battery_pin);
        void setSerial(BufferedSerial* serial);
        void set74HC165(_74HC165* hc165);

        void setX1(int8_t x1);
        void setY1(int8_t y1);
        void setX2(int8_t x2);
        void setY2(int8_t y2);

        void set_Up_flag(bool flag);
        void set_Down_flag(bool flag);
        void set_Left_flag(bool flag);
        void set_Right_flag(bool flag);

        void set_A_flag(bool flag);
        void set_B_flag(bool flag);
        void set_X_flag(bool flag);
        void set_Y_flag(bool flag);

        void set_L1_flag(bool flag);
        void set_L2_flag(bool flag);
        void set_R1_flag(bool flag);
        void set_R2_flag(bool flag);

        void set_LSW_flag(bool flag);
        void set_RSW_flag(bool flag);

        void set_estop_flag(bool flag);
        void set_reset_flag(bool flag);
        void set_home_flag(bool flag);

        bool get_Up_flag();
        bool get_Down_flag();
        bool get_Left_flag();
        bool get_Right_flag();

        bool get_A_flag();
        bool get_B_flag();
        bool get_X_flag();
        bool get_Y_flag();

        bool get_L1_flag();
        bool get_L2_flag();
        bool get_R1_flag();
        bool get_R2_flag();

        bool get_LSW_flag();
        bool get_RSW_flag();

        bool get_estop_flag();
        bool get_reset_flag();
        bool get_home_flag();

        void encode_joysticks();
        void decode_butts();
        void encode_op(int value);

        float get_volt();
        float get_volt_percentage();
        int8_t get_x1();
        int8_t get_y1();
        int8_t get_x2();
        int8_t get_y2();

        void refresh_msg();
        void send();

        int8_t to_percentage(int value, int max, int mid, int min);
        void cal_battery_volt();
};

#endif