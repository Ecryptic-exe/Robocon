#include <Arduino.h>
#include "IWatchdog.h"
#include "pinmap.h"
#include "communication_format.h"

HardwareSerial pc(uart1_rx, uart1_tx); //rx, tx
HardwareSerial BLE(uart2_rx, uart2_tx); //rx, tx

uint8_t up_flag, down_flag, Left_flag, Right_flag, A_flag, B_flag, X_flag, Y_flag, L1, L2, R1, R2, LjoystickSW, RjoystickSW, ESTOP, Reset_button, Home;

remote_msg in_msg;

void getRemote_packet(){
  in_msg = remote_msg();

  if((int8_t)BLE.read() != start_byte){
    return;
  }

  in_msg.x = BLE.read();
  in_msg.y = BLE.read();
  in_msg.w = BLE.read();
  in_msg.operation1 = (int8_t)BLE.read();
  in_msg.operation2 = (int8_t)BLE.read();
  in_msg.operation3 = (int8_t)BLE.read();

  up_flag = (uint8_t) (in_msg.operation1 & 0x01);
  down_flag = (uint8_t) ((in_msg.operation1 >> 1) & 0x01);
  Left_flag = (uint8_t) ((in_msg.operation1 >> 2) & 0x01);
  Right_flag = (uint8_t) ((in_msg.operation1 >> 3) & 0x01);
  A_flag = (uint8_t) ((in_msg.operation1 >> 4) & 0x01);
  B_flag = (uint8_t) ((in_msg.operation1 >> 5) & 0x01);
  X_flag = (uint8_t) ((in_msg.operation1 >> 6) & 0x01);
  Y_flag = (uint8_t) ((in_msg.operation1 >> 7) & 0x01);

  L1 = (uint8_t) (in_msg.operation2 & 0x01);
  L2 = (uint8_t) ((in_msg.operation2 >> 1) & 0x01);
  R1 = (uint8_t) ((in_msg.operation2 >> 2) & 0x01);
  R2 = (uint8_t) ((in_msg.operation2 >> 3) & 0x01);
  LjoystickSW = (uint8_t) ((in_msg.operation2 >> 4) & 0x01);
  RjoystickSW = (uint8_t) ((in_msg.operation2 >> 5) & 0x01);

  ESTOP = (uint8_t) (in_msg.operation3 & 0x01);
  Reset_button = (uint8_t) ((in_msg.operation3 >> 1) & 0x01);
  Home = (uint8_t) ((in_msg.operation3 >> 2) & 0x01);

  // pc.printf("x: %d, y: %d, w: %d, oper1:%d, oper2:%d, oper3:%d \r\n", in_msg.x, in_msg.y, in_msg.w, in_msg.operation1, in_msg.operation2, in_msg.operation3);
  // pc.printf("x: %d, y: %d, w: %d, up:%d, down:%d, left:%d, right:%d, A:%d, B:%d, X:%d, Y:%d, L1:%d, L2:%d, R1:%d, R2:%d, LSW:%d, RSW:%d, estop:%d, reser:%d, Home:%d \n\r", in_msg.x, in_msg.y, in_msg.w, up_flag, down_flag, Left_flag, Right_flag, A_flag, B_flag, X_flag, Y_flag, L1, L2, R1, R2, LjoystickSW, RjoystickSW, ESTOP, Reset_button, Home);
  // pc.puts((char*)&in_msg);
  // printf("hello world\r\n");
  // pc.printf("X: %d, Y: %d, W: %d\n", x, y, w);
}

void button(){
  // float maxpwm = 0.98f;
  // float minpwm = 0.02f;
  printf("up: %d, down: %d, left: %d, right: %d, A: %d, B: %d, X: %d, Y: %d \n", up_flag, down_flag, Left_flag, Right_flag, A_flag, B_flag, X_flag, Y_flag);
  if (ESTOP == 0x01)
  {
    
  }
  if (Home == 0x01)
  {
    
  }
  if (Reset_button == 0x01)
  {

  }
  if (L1 == 0x01)
  {

  }
  if (L2 == 0x01)
  {

  }
  if (R1 == 0x01)
  {

  }
  if (R2 == 0x01)
  {
    
  }

  if (up_flag == 0x01)
  {

  }

  if (down_flag == 0x01)
  {

  }
  
  if (Left_flag == 0x01)
  {

  }
  
  if (Right_flag == 0x01)
  {

  }
  
  if (A_flag == 0x01)
  {

  }
  
  if (B_flag == 0x01)
  {

  }
  
  if (X_flag == 0x01)
  {
 
  }

  if (Y_flag == 0x01)
  {

  }
}


void setup() {
  delay(1000);
  digitalWrite(BLE_Mod, HIGH);
  digitalWrite(BLE_reset, HIGH);
  BLE.begin(115200);
  pc.begin(115200);

  IWatchdog.begin(3000000); //3s
}

void loop() {
  getRemote_packet();
  button();
  // pc.printf("Hello World\r\n");
  IWatchdog.reload();
}