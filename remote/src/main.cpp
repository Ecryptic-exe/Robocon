#include <mbed.h>
#include "pinmapremote.h"

#include "communication_format.h"
#include "_74HC165.h"
// #include "Adafruit_GFX.h"
// #include "Adafruit_SSD1306.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "remote.h"
#include "Map.hpp"

char buff[256] = {0};

//monitor
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


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


I2C_HandleTypeDef hi2c1;

//////////////////////////////////////////////////////////////////
void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  // Configure the SCL and SDA pins
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /**I2C1 GPIO Configuration    
  PB6     ------> I2C1_SCL
  PB7     ------> I2C1_SDA 
  */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_I2C_Init(&hi2c1);
}
//////////////////////////////////////////////////////////////////




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


void display_voltage() {
  char volt_buffer[10]; 
  int voltage = controller.get_volt(); 


  sprintf(volt_buffer, "%d%%", voltage);


  ssd1306_SetCursor(0, 0);
  ssd1306_WriteString("V:", Font_6x8, White);
  ssd1306_WriteString(volt_buffer, Font_6x8, White);
  ssd1306_WriteString("V", Font_6x8, White);
}

void display_voltage_percentage() {
  char volt_per_buffer[10]; 
  int voltage = controller.get_volt_percentage(); 

  sprintf(volt_per_buffer, "%d%%", voltage);

  ssd1306_SetCursor(55, 0); 
  ssd1306_WriteString("V:", Font_6x8, White); 
  ssd1306_WriteString(volt_per_buffer, Font_6x8, White); 
  ssd1306_WriteString("%", Font_6x8, White); 
}

void display_analog_x(){
  char al_x[10];
  sprintf(al_x, "X:%d", controller.get_x1());

  ssd1306_SetCursor(0, 20);
  ssd1306_WriteString(al_x, Font_6x8, White);
}

void display_analog_y(){
  char al_y[10];
  sprintf(al_y, "Y:%d", controller.get_y1());

  ssd1306_SetCursor(45, 20);
  ssd1306_WriteString(al_y, Font_6x8, White);
}

void display_analog_w(){
  char al_w[10];
  sprintf(al_w, "Y:%d", controller.get_x1());

  ssd1306_SetCursor(90, 20);
  ssd1306_WriteString(al_w, Font_6x8, White);
}

void display_pressed_buttons(){

  char pressed_buttons[10];
  sprintf(pressed_buttons, "%d", pressed_butts);
  ssd1306_SetCursor(0, 40);
  ssd1306_WriteString("Pressed: ", Font_6x8, White);
  ssd1306_WriteString(pressed_buttons, Font_6x8, White);

}

void display_info(){
  ssd1306_Fill(Black);
  
  display_voltage();

  display_voltage_percentage();

  display_analog_x();

  display_analog_y();

  display_analog_w();

  display_pressed_buttons();

  ssd1306_UpdateScreen();
}


int main(){
  MX_I2C1_Init();
// Setup
  init_pinout();
  ssd1306_Init();

  BLE.set_baud(115200);
  BLE.set_format(
      /* bits */ 8,
      /* parity */ BufferedSerial::None,
      /* stop bit */ 1
  );

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

  // USB2TTL.printf("X: %d, Y: %d, W: %d\n", x, y, w);
  // USB2TTL.printf("robot_volt: %.2fV, rpm: %.2f, angle: %.2f\n", robot_volt, rpm, angle);
  }
    return 0;
}

