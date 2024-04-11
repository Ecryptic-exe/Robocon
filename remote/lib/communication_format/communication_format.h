#ifndef COMMUNICATION_FORMAT_H
#define COMMUNICATION_FORMAT_H

#include "mbed.h"

#define start_byte 0xff

struct remote_msg
{
  int8_t startByte = start_byte;
  int8_t senderID = 0x00;
  int8_t receiverID = 0x00;
  int8_t extension = 0x00; //For external plugins, set 0x01 for op4 and op5
  int8_t x = 0x00;
  int8_t y = 0x00;
  int8_t w = 0x00;
  uint8_t operation1 = 0x00; // operation1 Up=0, Down=1, Left=2, Right=3, A=4, B=5, X=6, Y=7,
  uint8_t operation2 = 0x00; // operation2 L1=0, L2=1, R1=2, R2=3, LjoystickSW=4, RjoystickSW=5, 
  uint8_t operation3 = 0x00; // Estop=0, Reset=1, hold=2
  uint8_t operation4 = 0x00; //Enable for external
  uint8_t operation5 = 0x00; //Enable for external
  int8_t CRC_high = 0x00;
  int8_t CRC_low = 0x00;
};

#endif