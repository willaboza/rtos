// gpio.c

// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// GPIO APB ports A-F

// Hardware configuration:
// 6 Pushbuttons and 5 LEDs, UART
// LEDS on these pins:
// Blue:   PF2 (on-board)
// Red:    PE1
// Orange: PE2
// Yellow: PE3
// Green:  PE4

// PBs on these pins
// PB0:    PA2
// PB1:    PA3
// PB2:    PA4
// PB3:    PA5
// PB4:    PA6
// PB5:    PA7

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------


#ifndef GPIO_H_
#define GPIO_H_

#include <stdbool.h>
#include "gpio.h"

#define RED_LED_ONBOARD   PORTF,1 // On-board red LED
#define BLUE_LED_ONBOARD  PORTF,2 // On-board blue LED
#define GREEN_LED_ONBOARD PORTF,3 // On-board green LED

#define RED_LED    PORTE,1 // Off-board red LED
#define ORANGE_LED PORTE,2 // Off-board orange LED
#define YELLOW_LED PORTE,3 // Off-board yellow LED
#define GREEN_LED  PORTE,4 // Off-board green LED
#define BLUE_LED   PORTE,5 // Off-board blue LED

#define PUSH_BUTTON_0 PORTA,2
#define PUSH_BUTTON_1 PORTA,3
#define PUSH_BUTTON_2 PORTA,4
#define PUSH_BUTTON_3 PORTA,5
#define PUSH_BUTTON_4 PORTA,6
#define PUSH_BUTTON_5 PORTA,7

// Enum values set to bitband address of GPIO_PORTx_DATA_R register
typedef enum _PORT
{
    PORTA = 0x42000000 + (0x400043FC-0x40000000)*32,
    PORTB = 0x42000000 + (0x400053FC-0x40000000)*32,
    PORTC = 0x42000000 + (0x400063FC-0x40000000)*32,
    PORTD = 0x42000000 + (0x400073FC-0x40000000)*32,
    PORTE = 0x42000000 + (0x400243FC-0x40000000)*32,
    PORTF = 0x42000000 + (0x400253FC-0x40000000)*32
} PORT;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void enablePort(PORT port);
void disablePort(PORT port);

void selectPinPushPullOutput(PORT port, uint8_t pin);
void selectPinOpenDrainOutput(PORT port, uint8_t pin);
void selectPinDigitalInput(PORT port, uint8_t pin);
void selectPinAnalogInput(PORT port, uint8_t pin);
void setPinCommitControl(PORT port, uint8_t pin);

void enablePinPullup(PORT port, uint8_t pin);
void disablePinPullup(PORT port, uint8_t pin);
void enablePinPulldown(PORT port, uint8_t pin);
void disablePinPulldown(PORT port, uint8_t pin);

void setPinAuxFunction(PORT port, uint8_t pin, uint32_t fn);

void selectPinInterruptRisingEdge(PORT port, uint8_t pin);
void selectPinInterruptFallingEdge(PORT port, uint8_t pin);
void selectPinInterruptBothEdges(PORT port, uint8_t pin);
void selectPinInterruptHighLevel(PORT port, uint8_t pin);
void selectPinInterruptLowLevel(PORT port, uint8_t pin);
void enablePinInterrupt(PORT port, uint8_t pin);
void disablePinInterrupt(PORT port, uint8_t pin);

void setPinValue(PORT port, uint8_t pin, bool value);
void togglePinValue(PORT port, uint8_t pin);
bool getPinValue(PORT port, uint8_t pin);
void setPortValue(PORT port, uint8_t value);
uint8_t getPortValue(PORT port);

void waitPbPress(PORT port1, uint8_t pin1, PORT port2, uint8_t pin2);

#endif /* GPIO_H_ */
