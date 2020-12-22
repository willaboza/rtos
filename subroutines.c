// subroutines.c

// William Bozarth
// Created on: December 21, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "subroutines.h"
#include "gpio.h"
#include "wait.h"

// Initialize Hardware
// REQUIRED: Add initialization for blue, orange, red, green, and yellow LEDs
//           6 pushbuttons, and uart
void initHw(void)
{
    // Configure HW to work with 16 MHz XTAL, PLL enabled, sysdivider of 5, creating system clock of 40 MHz
    SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);
    _delay_cycles(3);

    // Enable clocks
    enablePort(PORTA);
    enablePort(PORTE);
    enablePort(PORTF);

    // Configure LEDs
    selectPinPushPullOutput(RED_LED_ONBOARD);
    selectPinPushPullOutput(BLUE_LED_ONBOARD);
    selectPinPushPullOutput(GREEN_LED_ONBOARD);
    selectPinPushPullOutput(RED_LED);
    selectPinPushPullOutput(ORANGE_LED);
    selectPinPushPullOutput(YELLOW_LED);
    selectPinPushPullOutput(GREEN_LED);
    selectPinPushPullOutput(BLUE_LED);

    // Configure push-buttons
    selectPinDigitalInput(PUSH_BUTTON_0);
    selectPinDigitalInput(PUSH_BUTTON_1);
    selectPinDigitalInput(PUSH_BUTTON_2);
    selectPinDigitalInput(PUSH_BUTTON_3);
    selectPinDigitalInput(PUSH_BUTTON_4);
    selectPinDigitalInput(PUSH_BUTTON_5);
}

// REQUIRED: add code to return a value from 0-63 indicating which of 6 PBs are pressed
uint8_t readPbs(void)
{
    return 0;
}

//-----------------------------------------------------------------------------
// YOUR UNIQUE CODE
// REQUIRED: add any custom code in this space
//-----------------------------------------------------------------------------

void testLeds(void)
{
    setPinValue(RED_LED, 1);
    waitMicrosecond(1000000);
    setPinValue(RED_LED, 0);
    waitMicrosecond(1000000);

    setPinValue(ORANGE_LED, 1);
    waitMicrosecond(1000000);
    setPinValue(ORANGE_LED, 0);
    waitMicrosecond(1000000);

    setPinValue(YELLOW_LED, 1);
    waitMicrosecond(1000000);
    setPinValue(YELLOW_LED, 0);
    waitMicrosecond(1000000);

    setPinValue(GREEN_LED, 1);
    waitMicrosecond(1000000);
    setPinValue(GREEN_LED, 0);
    waitMicrosecond(1000000);

    setPinValue(BLUE_LED, 1);
    waitMicrosecond(1000000);
    setPinValue(BLUE_LED, 0);
    waitMicrosecond(1000000);
}

void testPbs(void)
{
    waitPbPress(PUSH_BUTTON_0, ORANGE_LED);
    waitPbPress(PUSH_BUTTON_1, BLUE_LED);
    waitPbPress(PUSH_BUTTON_2, GREEN_LED);
    waitPbPress(PUSH_BUTTON_3, YELLOW_LED);
    waitPbPress(PUSH_BUTTON_4, RED_LED);
    waitPbPress(PUSH_BUTTON_5, ORANGE_LED);
}
