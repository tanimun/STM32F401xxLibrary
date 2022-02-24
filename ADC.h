
#ifndef GPIO_INCLUDE_H
#define GPIO_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"                  // Device header
//void pinMode(uint8_t port,uint8_t mode,uint16_t pin );
//void digitalWrite(uint8_t port,unsigned int PORT_pin , int state);

typedef void * PinName;

#define PIN_0   0x0000
#define PIN_1   0x0001
#define PIN_2   0x0002
#define PIN_3   0x0003
#define PIN_4   0x0004
#define PIN_5   0x0005
#define PIN_6   0x0006
#define PIN_7   0x0007
#define PIN_8   0x0008
#define PIN_9   0x0009
#define PIN_10  0x000A
#define PIN_11  0x000B
#define PIN_12  0x000C
#define PIN_13  0x000D
#define PIN_14  0x000E
#define PIN_15  0x000F


#define PORTA 0x0000
#define PORTB 0x0001
#define PORTC 0x0002
#define PORTD 0x0003
#define PORTE 0x0004
#define PORTH 0x0006


///////////////////direction////////////
#define INPUT       0
#define OUTPUT      1
#define ALTF        2
#define ANALOG_MODE 3
//////////////////pin state////////
#define HIGH   1
#define LOW    0


PinName PinCreate(uint8_t port, uint16_t pin, uint8_t mode);
void digitalWrite_High(PinName pin);
void digitalWrite_Low(PinName pin);
uint16_t analogRead(PinName pin);

#endif  // GPIO_INCLUDE_H
