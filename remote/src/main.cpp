#include <mbed.h>
#include "pinmapremote.h"

#include "communication_format.h"
#include "_74HC165.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "remote.h"
#include "Map.hpp"

char buff[256] = {0};

//monitor
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
I2C i2c1(I2C1_SDA, I2C1_SCL);
Adafruit_SSD1306_I2c display(&i2c1, -1, 0x78, SCREEN_HEIGHT, SCREEN_WIDTH);


uint32_t pre_time = 0;
uint32_t now_time = 0;
int off_time_ms = 3000;

//butts status
string pressed_butts = "";

//remote
remote controller;

Timer timer;

// robot_msg in_msg;

//feedback var from robot
float robot_volt = 0.0f;
float rpm = 0.0f; //common var
float angle = 0.0f; //common var
string setting_str = "";  //common var

void turnOn(){
  PG2 = 1;
  LED_Y = 1;
}

void turnOff(){
  PG2 = 0;
  LED_Y = 0;
}

void init_pinout(){ 
  turnOn();

_74HC165 butt_data(BUTTON_DATA_PIN, CP, nPL, 8);;

  //init controller
  controller.set74HC165(&butt_data);
  controller.setSerial(&BLE);
  controller.setJoystickPins(X1, Y1, X2, Y2);
  controller.setHomePin(PG1);
  controller.setBattery_Pin(BAT_LV);
}

void init_oled(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x78)) {
        const char *error_msg = "SSD1306 allocation failed";
        USB2TTL.write(error_msg, strlen(error_msg));
        for(;;);
    }
  // Clear the buffer
  display.clearDisplay();
}

void check_off_cmd() {
    now_time = Kernel::get_ms_count(); 

    // LOW = pressed
    if(!Home.read()){ // Mbed equivalent of digitalRead
        // Check off command
        if(now_time - pre_time >= off_time_ms){
            turnOff();
        }
    }
    else{
        pre_time = now_time;
    }
}

void updated_pressed_str(string name, bool value){
  if(value){
    pressed_butts += name;
    pressed_butts += " ";
  }
}

void encode_butts_str(){
  //clear
  pressed_butts = "";

  updated_pressed_str("up", controller.get_Up_flag());
  updated_pressed_str("left", controller.get_Left_flag());
  updated_pressed_str("down", controller.get_Down_flag());
  updated_pressed_str("right", controller.get_Right_flag());

  updated_pressed_str("X", controller.get_X_flag());
  updated_pressed_str("Y", controller.get_Y_flag());
  updated_pressed_str("A", controller.get_A_flag());
  updated_pressed_str("B", controller.get_B_flag());

  updated_pressed_str("L_sw", controller.get_LSW_flag());
  updated_pressed_str("R_sw", controller.get_RSW_flag());
  updated_pressed_str("Estop", controller.get_estop_flag());
  updated_pressed_str("Reset", controller.get_reset_flag());
  updated_pressed_str("Home", controller.get_home_flag());

  updated_pressed_str("R1", controller.get_R1_flag());
  updated_pressed_str("R2", controller.get_R2_flag());
  updated_pressed_str("L1", controller.get_L1_flag());
  updated_pressed_str("L2", controller.get_L2_flag());

  //for debug
  // USB2TTL.printf("up: %d, left: %d, down: %d, right: %d, X: %d, Y: %d, A: %d, B: %d\n", up, left, down, right, X, Y, A, B);
  // USB2TTL.printf("L_switch: %d, R_switch: %d, Estop: %d, Reset: %d, Home: %d\n", L_switch, R_switch, Estop, Reset, Home);
  // USB2TTL.printf("R1: %d, R2: %d, L1: %d, L2: %d\n", R1, R2, L1, L2);
}

void decode_robotMsg(){
  //reflash
  //debug hidden
  // in_msg = robot_msg();

  if((int8_t)BLE.read(buff, sizeof(buff)) != (int8_t)start_byte){
    LED_G = 0;
    return;
  }

  LED_G = 1;

//Debug 1
  // in_msg.voltage[0] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.voltage[1] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.voltage[2] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.voltage[3] = (uint8_t)BLE.read(buff, sizeof(buff));

  // in_msg.rpm[0] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.rpm[1] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.rpm[2] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.rpm[3] = (uint8_t)BLE.read(buff, sizeof(buff));

  // in_msg.angle[0] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.angle[1] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.angle[2] = (uint8_t)BLE.read(buff, sizeof(buff));
  // in_msg.angle[3] = (uint8_t)BLE.read(buff, sizeof(buff));

  // in_msg.setting_num = (int8_t)BLE.read(buff, sizeof(buff));
  // in_msg.error_Flag = (int8_t)BLE.read(buff, sizeof(buff));

  //USB2TTL.printf("%.2x, %.2x, %.2x, %.2x\n", in_msg.angle[0], in_msg.angle[1], in_msg.angle[2], in_msg.angle[3]);

  // robot_volt = *(float*)(in_msg.voltage);
  // rpm = *(float*)(in_msg.rpm);
  // angle = *(float*)(in_msg.angle);

//   switch (in_msg.setting_num){
//     case (int8_t)0x01:
//       setting_str = "Pole 1";
//       break;
    
//     case (int8_t)0x02:
//       setting_str = "Near Pole 2";
//       break;

//     case (int8_t)0x03:
//       setting_str = "Far Pole 2";
//       break;

//     case (int8_t)0x04:
//       setting_str = "Pole 3";
//       break;
    
//     default:
//       setting_str = "Custom";
//       break;
//   }

//   switch(in_msg.error_Flag){
//     case (int8_t)0x01:
//       break;

//     default:
//       break;
//   }
}

void display_info(){
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  display.setTextCursor(0, 0);
  display.printf("V: ");
  display.printf("%d",controller.get_volt(), 2);
  display.printf("V");

  display.setTextCursor(55, 0);
  display.printf("%d",controller.get_volt_percentage(), 2);
  display.printf("%");

  display.setTextCursor(0, 20);
  display.printf("X:%d", controller.get_x1());
  display.setTextCursor(45, 20);
  display.printf("Y:%d", controller.get_y1());
  display.setTextCursor(90, 20);
  display.printf("W:%d", controller.get_x2());

  display.setTextCursor(0, 40);
  display.printf("Pressed: ");
  display.printf("%d",pressed_butts);

  display.display(); 
}




int main(){
// Setup
  init_pinout();
  init_oled();

  BLE.set_baud(115200);
  BLE.set_format(
      /* bits */ 8,
      /* parity */ BufferedSerial::None,
      /* stop bit */ 1
  );

  i2c1.frequency(100000); // 100kHz

  while (true){
    // Loop
      check_off_cmd();

  controller.refresh_msg();
  controller.encode_joysticks();
  controller.decode_butts();
  controller.encode_op(1);
  controller.encode_op(2);
  controller.encode_op(3);
  controller.send();

  encode_butts_str();

  //get the feedback data from robot
  decode_robotMsg();
  controller.cal_battery_volt();
  display_info();

  //for debug
  // int x = analogRead(X1_pin);
  // int y = analogRead(Y1_pin);
  // int w = analogRead(X2_pin);

  //USB2TTL.printf("X: %d, Y: %d, W: %d\n", x, y, w);
  //USB2TTL.printf("robot_volt: %.2fV, rpm: %.2f, angle: %.2f\n", robot_volt, rpm, angle);
  }
    return 0;
}

