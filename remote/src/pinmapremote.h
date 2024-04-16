#ifndef PINMAP_H
#define PINMAP_H

#include "mbed.h"
#include "_74HC165.h"

// #define BOOT0 BOOT0
// #define PA0 PA_0
#define BAT_LV PA_1
#define UART1_TX PA_2
#define UART1_RX PA_3
#define X1 PA_4
#define Y1 PA_5
#define X2 PA_6
#define Y2 PA_7
// #define LED_R PA_8
#define UART2_TX PA_9 //BT TX
#define UART2_RX PA_10 //BT RX
// #define JUMPER_PA11 PA_11
// #define JUMPER_PA12 PA_12
// #define SWDIO PA_13
// #define SWCLK PA_14
// #define nCE PA_15

// #define PB0 PB_0
// #define BLE_MOD PB_1
// #define BOOT1 PB_2
#define CP PB_3
#define nPL PB_4
#define BUTTON_DATA_PIN PB_5
#define I2C1_SCL PB_6 //Display
#define I2C1_SDA PB_7 //Display
// #define PG2 PB_8
#define PG1 PB_9
// #define PB10 PB_10
// #define PB12 PB_12
// #define BUZZER PB_13
// #define LED_G PB_14
// #define LED_Y PB_15

// #define JUMPER_PC0 PC_0
// #define JUMPER_PC1 PC_1
// #define JUMPER_PC2 PC_2
// #define JUMPER_PC3 PC_3
// #define JUMPER_PC4 PC_4
// #define JUMPER_PC5 PC_5
// #define JUMPER_PC6 PC_6
// #define JUMPER_PC7 PC_7
// #define JUMPER_PC8 PC_8
// #define JUMPER_PC9 PC_9
// #define JUMPER_PC10 PC_10
// #define JUMPER_PC11 PC_11
// #define BLE_RST PC_12
// #define PC13 PC_13

#define UART1_BAUD 115200
#define UART2_BAUD 115200

BufferedSerial BLE(UART2_TX, UART2_RX, UART1_BAUD);
BufferedSerial USB2TTL(UART1_TX, UART1_RX, UART2_BAUD);

I2C i2c1(I2C1_SDA, I2C1_SCL);

// DigitalOut BOOT0(BOOT_0);

DigitalIn PA0(PA_0);
// AnalogIn BAT_LV(PA_1);
// AnalogIn X1(PA_4);
// AnalogIn Y1(PA_5);
// AnalogIn X2(PA_6);
// AnalogIn Y2(PA_7);
DigitalOut LED_R(PA_8);
AnalogIn JUMPER_PA11(PA_11);
AnalogIn JUMPER_PA12(PA_12);
DigitalInOut SWDIO(PA_13);
DigitalInOut SWCLK(PA_14);
DigitalOut nCE(PA_15);

AnalogIn PB0 (PB_0);
DigitalOut BLE_MOD (PB_1);
DigitalOut BOOT1 (PB_2);
// DigitalOut CP (PB_3);
// DigitalOut nPL (PB_4);
// DigitalIn BUTTON_DATA_PIN (PB_5);
DigitalOut PG2 (PB_8);
// DigitalIn PG1 (PB_9);
DigitalIn Home (PG1);
AnalogIn PB10 (PB_10);
AnalogIn PB12 (PB_12);
DigitalOut BUZZER (PB_13);
DigitalOut LED_G (PB_14);
DigitalOut LED_Y (PB_15);

DigitalInOut JUMPER_PC0 (PC_0);
DigitalInOut JUMPER_PC1 (PC_1);
DigitalInOut JUMPER_PC2 (PC_2);
DigitalInOut JUMPER_PC3 (PC_3);
DigitalInOut JUMPER_PC4 (PC_4);
DigitalInOut JUMPER_PC6 (PC_6);
DigitalInOut JUMPER_PC7 (PC_7);
DigitalInOut JUMPER_PC8 (PC_8);
DigitalInOut JUMPER_PC9 (PC_9);
DigitalInOut JUMPER_PC10 (PC_10);
DigitalInOut JUMPER_PC11 (PC_11);
DigitalOut BLE_RST (PC_12);
DigitalOut PC13 (PC_13);

#endif