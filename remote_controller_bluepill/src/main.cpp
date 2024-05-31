#include <Arduino.h>
#include "IWatchdog.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pinmap.h"
#include "communication_format.h"
#include "_74HC165.h"
#include "remote.h"

//monitor
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
TwoWire i2c1(I2C1_SDA, I2C1_SCL);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &i2c1, -1);

HardwareSerial pc(uart1_rx, uart1_tx); //rx, tx
HardwareSerial BLE(uart2_rx, uart2_tx); //rx, tx

uint32_t pre_time = 0;
uint32_t now_time = 0;
int off_time_ms = 3000;

//butts status
int numBits = 16;
_74HC165 butt_data;
String pressed_butts = "";

//remote
remote controller;

HardwareTimer timer;

robot_msg in_msg;


//feedback var from robot
float robot_volt = 0.0f;
float rpm = 0.0f; //common var
float angle = 0.0f; //common var
String setting_str = "";  //common var


void turnOn(){
  digitalWrite(PG2, HIGH);
  digitalWrite(led2, HIGH);
}

void turnOff(){
  digitalWrite(PG2, LOW);
  digitalWrite(led2, LOW);
}

void init_pinout(){ 
  pinMode(PG1, INPUT);
  pinMode(PG2, OUTPUT);
  
  pinMode(butts_data_pin, INPUT);
  pinMode(nPL, OUTPUT);
  pinMode(CP, OUTPUT);
  pinMode(nCE, OUTPUT);

  pinMode(X1_pin, INPUT_ANALOG);
  pinMode(Y1_pin, INPUT_ANALOG);
  pinMode(X2_pin, INPUT_ANALOG);
  pinMode(Y2_pin, INPUT_ANALOG);

  pinMode(variable_r, INPUT_ANALOG);

  pinMode(battery_level, INPUT_ANALOG);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(BLE_reset, OUTPUT);
  pinMode(BLE_Mod, OUTPUT);

  turnOn();

  analogReadResolution(12);

  butt_data = _74HC165(nPL, CP, nCE, butts_data_pin, numBits);

  //init controller
  controller.set74HC165(&butt_data);
  controller.setSerial(&BLE);
  controller.setJoystickPins(X1_pin, Y1_pin, X2_pin, Y2_pin);
  controller.setHomePin(PG1);
  controller.setBattery_Pin(battery_level);

  i2c1.begin();
}

void init_oled(){
  // Init OLED display on bus number 0
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    pc.printf(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();
}

void init_BLE(){
  digitalWrite(BLE_Mod, LOW);
  delay(1500);
  // BLE.write("AT+AUTO_CNT=1, 60:B6:47:F3:2A:99 \r\n");
}

void check_off_cmd(){
  now_time = millis();

  // LOW = pressed
  if(!digitalRead(PG1)){
    //check off command
    if(now_time - pre_time >= off_time_ms){
      turnOff();
    }
  }
  else{
    pre_time = now_time;

  }
}

void updated_pressed_str(String name, bool value){
  if(value){
    pressed_butts += name;
    pressed_butts += " ";
  }
}

void debug_butts(){
  bool up = controller.get_Up_flag();
  bool left = controller.get_Left_flag();
  bool down = controller.get_Down_flag();
  bool right = controller.get_Right_flag();

  bool X = controller.get_X_flag();
  bool Y = controller.get_Y_flag();
  bool A = controller.get_A_flag();
  bool B = controller.get_B_flag();

  bool L_switch = controller.get_LSW_flag();
  bool R_switch = controller.get_RSW_flag();
  bool Estop = controller.get_estop_flag();
  bool Reset = controller.get_reset_flag();
  bool Home = controller.get_home_flag();

  bool R1 = controller.get_R1_flag();
  bool R2 = controller.get_R2_flag();
  bool L1 = controller.get_L1_flag();
  bool L2 = controller.get_L2_flag();

  pc.printf("up: %d, left: %d, down: %d, right: %d, X: %d, Y: %d, A: %d, B: %d \r\n", up, left, down, right, X, Y, A, B);
  pc.printf("L_switch: %d, R_switch: %d, Estop: %d, Reset: %d, Home: %d \r\n", L_switch, R_switch, Estop, Reset, Home);
  pc.printf("R1: %d, R2: %d, L1: %d, L2: %d \r\n", R1, R2, L1, L2);
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

  // debug_butts();
}

void decode_robotMsg(){
  //reflesh
  in_msg = robot_msg();

  if((int8_t)BLE.read() != (int8_t)start_byte){
    digitalWrite(led3, LOW);
    return;
  }

  digitalWrite(led3, HIGH);

  in_msg.voltage[0] = (uint8_t)BLE.read();
  in_msg.voltage[1] = (uint8_t)BLE.read();
  in_msg.voltage[2] = (uint8_t)BLE.read();
  in_msg.voltage[3] = (uint8_t)BLE.read();

  in_msg.rpm[0] = (uint8_t)BLE.read();
  in_msg.rpm[1] = (uint8_t)BLE.read();
  in_msg.rpm[2] = (uint8_t)BLE.read();
  in_msg.rpm[3] = (uint8_t)BLE.read();

  in_msg.angle[0] = (uint8_t)BLE.read();
  in_msg.angle[1] = (uint8_t)BLE.read();
  in_msg.angle[2] = (uint8_t)BLE.read();
  in_msg.angle[3] = (uint8_t)BLE.read();

  in_msg.setting_num = (int8_t)BLE.read();
  in_msg.error_Flag = (int8_t)BLE.read();

  pc.printf("%.2x, %.2x, %.2x, %.2x \r\n", in_msg.angle[0], in_msg.angle[1], in_msg.angle[2], in_msg.angle[3]);

  robot_volt = *(float*)(in_msg.voltage);
  rpm = *(float*)(in_msg.rpm);
  angle = *(float*)(in_msg.angle);

  switch (in_msg.setting_num){
    case (int8_t)0x01:
      setting_str = "Pole 1";
      break;
    
    case (int8_t)0x02:
      setting_str = "Near Pole 2";
      break;

    case (int8_t)0x03:
      setting_str = "Far Pole 2";
      break;

    case (int8_t)0x04:
      setting_str = "Pole 3";
      break;
    
    default:
      setting_str = "Custom";
      break;
  }

  switch(in_msg.error_Flag){
    case (int8_t)0x01:
      break;

    default:
      break;
  }
}

void display_info1(){
  //mon1
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  display.setCursor(0, 0);
  display.print("V: ");
  display.print(controller.get_volt(), 2);
  display.print("V");

  display.setCursor(55, 0);
  display.print(controller.get_volt_percentage(), 2);
  display.print("%");

  display.setCursor(0, 20);
  display.printf("X:%d", controller.get_x1());
  display.setCursor(45, 20);
  display.printf("Y:%d", controller.get_y1());
  display.setCursor(90, 20);
  display.printf("W:%d", controller.get_x2());

  display.setCursor(0, 40);
  display.print("Pressed: ");
  display.print(pressed_butts);

  display.display(); 
}

void display_info2(){
//mon2
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  display.setCursor(0, 0);
  display.print("V: ");
  display.print(robot_volt, 2);
  display.print("V");
  
  display.setCursor(0, 15);
  display.print("Angle: ");
  display.print(angle, 2);

  display.setCursor(0, 30);
  display.print("RPM: ");
  display.print(rpm, 2);

  display.setCursor(0, 45);
  display.print("Setting: ");
  display.print(setting_str);

  display.display(); 
}


void setup() {
  init_pinout();
  init_oled();

  digitalWrite(BLE_Mod, HIGH);
  digitalWrite(BLE_reset, HIGH);
  BLE.begin(115200);

  // init_BLE();
  //for debug
  pc.begin(115200);
  
  //watchdog avoid lagging
  IWatchdog.begin(3000000); //3s

}



void loop() {
  check_off_cmd();

// //test function
//   digitalWrite(BLE_Mod,LOW);
//   delay(3000);
//   if (digitalRead(BLE_Mod==LOW)){
//     BLE.write("AT\r\n");
//   }
  
    // String data = BLE.readString();
    // pc.print("Data received: ");
    // pc.print(data);
    // BLE.write("test send\r\n");



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

  //Switch page with home
  if(controller.get_home_flag()){
    display_info2();
  }
  else display_info1(); 

  IWatchdog.reload();

// BLE Reset Debug
  // if (digitalRead(BLE_reset)==LOW){
  //   pc.printf("BLE Reset");
  // }

  //for debug
  // int x = analogRead(X1_pin);
  // int y = analogRead(Y1_pin);
  // int w = analogRead(X2_pin);

  // pc.printf("X: %d, Y: %d, W: %d \r\n", x, y, w);
  // pc.printf("robot_volt: %.2fV, rpm: %.2f, angle: %.2f\n", robot_volt, rpm, angle);
}
