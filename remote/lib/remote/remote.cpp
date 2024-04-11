#include "remote.h"

void remote::setJoystickPins(int x1_pin, int y1_pin, int x2_pin, int y2_pin){
    this->x1_pin = x1_pin;
    this->y1_pin = y1_pin;
    this->x2_pin = x2_pin;
    this->y2_pin = y2_pin;
}
void remote::setHomePin(int home_pin){
    this->home_pin = home_pin;
}
void remote::setBattery_Pin(int battery_pin){
    this->battery_level_pin = battery_pin;
}
void remote::setSerial(BufferedSerial* serial){
    this->serial = serial;
}
void remote::set74HC165(_74HC165* hc165){
    this->hc165 = hc165;
}

void remote::setX1(int8_t x1){
    this->x1 = x1;

    if(this->x1 > 100){
        this->x1 = 100;
    }

    if(this->x1 < -100){
        this->x1 = -100;
    }
}

void remote::setY1(int8_t y1){
    this->y1 = y1;

    if(this->y1 > 100){
        this->y1 = 100;
    }

    if(this->y1 < -100){
        this->y1 = -100;
    }
}

void remote::setX2(int8_t x2){
    this->x2 = x2;

    if(this->x2 > 100){
        this->x2 = 100;
    }

    if(this->x2 < -100){
        this->x2 = -100;
    }
}

void remote::setY2(int8_t y2){
    this->y2 = y2;

    if(this->y2 > 100){
        this->y2 = 100;
    }

    if(this->y2 < -100){
        this->y2 = -100;
    }
}

void remote::set_Up_flag(bool flag){
    up_flag = flag;
}
void remote::set_Down_flag(bool flag){
    down_flag = flag;
}
void remote::set_Left_flag(bool flag){
    Left_flag = flag;
}
void remote::set_Right_flag(bool flag){
    Right_flag = flag;
}

void remote::set_A_flag(bool flag){
    A_flag = flag;
}
void remote::set_B_flag(bool flag){
    B_flag = flag;
}
void remote::set_X_flag(bool flag){
    X_flag = flag;
}
void remote::set_Y_flag(bool flag){
    Y_flag = flag;
}

void remote::set_L1_flag(bool flag){
    L1 = flag;
}
void remote::set_L2_flag(bool flag){
    L2 = flag;
}
void remote::set_R1_flag(bool flag){
    R1 = flag;
}
void remote::set_R2_flag(bool flag){
    R2 = flag;
}

void remote::set_LSW_flag(bool flag){
    LjoystickSW = flag;
}
void remote::set_RSW_flag(bool flag){
    RjoystickSW = flag;
}

void remote::set_estop_flag(bool flag){
    ESTOP = flag;
}
void remote::set_reset_flag(bool flag){
    Reset_button = flag;
}
void remote::set_home_flag(bool flag){
    Home = flag;
}

bool remote::get_Up_flag(){
    return up_flag;
}
bool remote::get_Down_flag(){
    return down_flag;
}
bool remote::get_Left_flag(){
    return Left_flag;
}
bool remote::get_Right_flag(){
    return Right_flag;
}

bool remote::get_A_flag(){
    return A_flag;
}
bool remote::get_B_flag(){
    return B_flag;
}
bool remote::get_X_flag(){
    return X_flag;
}
bool remote::get_Y_flag(){
    return Y_flag;
}

bool remote::get_L1_flag(){
    return L1;
}
bool remote::get_L2_flag(){
    return L2;
}
bool remote::get_R1_flag(){
    return R1;
}
bool remote::get_R2_flag(){
    return R2;
}

bool remote::get_LSW_flag(){
    return LjoystickSW;
}
bool remote::get_RSW_flag(){
    return RjoystickSW;
}

bool remote::get_estop_flag(){
    return ESTOP;
}
bool remote::get_reset_flag(){
    return Reset_button;
}
bool remote::get_home_flag(){
    return Home;
}

float remote::get_volt(){
    return volt;
}
float remote::get_volt_percentage(){
    return volt_percentage;
}
int8_t remote::get_x1(){
    return x1;
}
int8_t remote::get_y1(){
    return y1;
}
int8_t remote::get_x2(){
    return x2;
}
int8_t remote::get_y2(){
    return y2;
}

void remote::encode_joysticks() {
    // Create AnalogIn objects for each pin
    AnalogIn x1_analog(x1_pin);
    AnalogIn y1_analog(y1_pin);
    AnalogIn x2_analog(x2_pin);

    // Read the analog values and convert to percentage
    msg.x = to_percentage(x1_analog.read(), x_min, x_mid, x_max);  // reverse
    msg.y = to_percentage(y1_analog.read(), y_max, y_mid, y_min);
    msg.w = to_percentage(x2_analog.read(), w_min, w_mid, w_max);  // reverse

    // Assign the values to your variables
    int x1 = msg.x;
    int y1 = msg.y;
    int x2 = msg.w;

    // Use printf for serial output
    // printf("X: %d, Y: %d, W: %d\n", x1, y1, x2);
}

void remote::decode_butts(){

    data = new bool[hc165->getNumBits()];
    data = hc165->read();

    DigitalIn HomePin(home_pin);

    Home = !HomePin.read();

    up_flag = data[0];
    Left_flag = data[1];
    down_flag = data[2];
    Right_flag = data[3];

    X_flag = data[4];
    Y_flag = data[5];
    A_flag = data[6];
    B_flag = data[7];

    LjoystickSW = data[8];
    RjoystickSW = data[9];
    ESTOP = data[10];
    Reset_button = data[11];

    R1 = data[12];
    R2 = data[13];
    L1 = data[14];
    L2 = data[15];
}

void remote::encode_op(int value){
    switch(value){
        case 1:
            //operation 1
            // operation1 Up=0, Down=1, Left=2, Right=3, A=4, B=5, X=6, Y=7,
            msg.operation1 |= (int8_t)((int8_t)Y_flag << 7);
            msg.operation1 |= (int8_t)((int8_t)X_flag << 6);
            msg.operation1 |= (int8_t)((int8_t)B_flag << 5);
            msg.operation1 |= (int8_t)((int8_t)A_flag << 4);
            msg.operation1 |= (int8_t)((int8_t)Right_flag << 3);
            msg.operation1 |= (int8_t)((int8_t)Left_flag << 2);
            msg.operation1 |= (int8_t)((int8_t)down_flag << 1);
            msg.operation1 |= (int8_t)up_flag;
            break;

        case 2:
            //operation 2
            // operation2 L1=0, L2=1, R1=2, R2=3, LjoystickSW=4, RjoystickSW=5,
            msg.operation2 |= (int8_t)((int8_t)RjoystickSW << 5);
            msg.operation2 |= (int8_t)((int8_t)LjoystickSW << 4);
            msg.operation2 |= (int8_t)((int8_t)R2 << 3);
            msg.operation2 |= (int8_t)((int8_t)R1 << 2);
            msg.operation2 |= (int8_t)((int8_t)L2 << 1);
            msg.operation2 |= (int8_t)L1;
            break;

        case 3:
            //operation 3
            // Estop=0, Reset=1, Home=2
            msg.operation3 |= (int8_t)((int8_t)Home << 2);
            msg.operation3 |= (int8_t)((int8_t)Reset_button << 1);
            msg.operation3 |= (int8_t)ESTOP;
            break;

        default:
            break;
    }
}

void remote::refresh_msg(){
    msg = remote_msg();
}

void remote::send(){
    serial->write((char*)&msg, sizeof(msg));
    serial->sync();
}

int8_t remote::to_percentage(int value, int max, int mid, int min){
  int temp = value;

  //deadzone
  if(abs(temp-mid) <= allow_offset){
    return (int8_t)0;
  }

  //limitation
  if(temp > max){
    temp = max;
  }

  if(temp < min){
    temp = min;
  }

  //map(value, fromLow, fromHigh, toLow, toHigh)
  Map temp(min, max, -100, 100);
  return (int8_t)temp.Calculate(value);
}

void remote::cal_battery_volt(){

    AnalogIn BatteryLv(battery_level_pin);

    int temp = BatteryLv.read();

    //pc.println(temp);

    int resistor_1 = 100000;
    int resistor_2 = 10000;



    volt = (float)((temp*(3.3f/4096.0f))*((resistor_1 + resistor_2)/resistor_2));
    Map volt(volt_min*1000, volt_max*1000, 0.0f, 100.0f*1000)
    volt_percentage = volt.Calculate(volt*1000)/1000.0f;

}

