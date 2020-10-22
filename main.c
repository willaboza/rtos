// main.c
// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "uart0.h"
#include "shell.h"
#include "reboot.h"
#include "wait.h"
#include "rtos.h"

#define RED_LED PORTF,1
#define GREEN_LED PORTF,3

void initHw(void);

//
// Start of Main Function
//
int main(void){

    // Initialize hardware
    initHw();
    initUart0();
    // initWatchdog();

    // Setup UART0 Baud Rate
    setUart0BaudRate(115200, 40e6);

    // Declare Variables
    USER_DATA userInput = {0};

    //test green LED is functioning correctly (turn on and then off)
    setPinValue(GREEN_LED, 1);
    waitMicrosecond(100000);
    setPinValue(GREEN_LED, 0);
    waitMicrosecond(100000);

    //Print Main Menu
    printMainMenu();

    //stay in while loop until program is exited to get next input from the terminal
    while(true)
    {
        // Re-set user input
        resetUserInput(&userInput);

        // Spin until user input if complete
        while(!userInput.endOfString)
        {
            // If User Input detected, then process input
            if(kbhitUart0())
            {
                // Get User Input
                getsUart0(&userInput);

                // Tokenize User Input
                parseFields(&userInput);
            }
        }

        // Perform Command from User Input
        if(isCommand(&userInput, "reboot", 1))
        {
            rebootFlag = true; // Set flag for controller reboot
        }
        else if(isCommand(&userInput, "ps", 1))
        {
            ps();
        }
        else if(isCommand(&userInput, "ipcs", 1))
        {
            ipcs();
        }
        else if(isCommand(&userInput, "kill", 2))
        {
            int pid = getFieldInteger(&userInput, 1);

            kill(pid);
        }
        else if(isCommand(&userInput, "pi", 2))
        {
            char buffer[10];

            getFieldString(&userInput, buffer, 1);

            if(strcmp(buffer, "on") == 0)
            {
                pi(1);
            }
            else if(strcmp(buffer, "off") == 0)
            {
                pi(0);
            }
        }
        else if(isCommand(&userInput, "preempt", 2))
        {
            char buffer[10];

            getFieldString(&userInput, buffer, 1);

            if(strcmp(buffer, "on") == 0)
            {
                preempt(1);
            }
            else if(strcmp(buffer, "off") == 0)
            {
                preempt(0);
            }
        }
        else if(isCommand(&userInput, "sched", 2))
        {
            char buffer[10];

            getFieldString(&userInput, buffer, 1);

            if(strcmp(buffer, "prio") == 0)
            {
                sched(1);
            }
            else if(strcmp(buffer, "rr") == 0)
            {
                sched(0);
            }
        }
        else if(isCommand(&userInput, "pidof", 2)) // Configures the hardware to send an RGB triplet immediately
        {
            char buffer[10];

            getFieldString(&userInput, buffer, 1);

            pidof(buffer);
        }
        else if(isCommand(&userInput, "run", 2)) // Configures the hardware to send an RGB triplet when the PB is pressed
        {
            char buffer[10];

            getFieldString(&userInput, buffer, 1);

            setPinValue(RED_LED, 1);
        }
    }
}

// Function to Initialize System Clock
void initHw(void)
{
    // Configure HW to work with 16 MHz XTAL, PLL enabled, sysdivider of 5, creating system clock of 40 MHz
    SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S) | SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_2;

    // Enable clocks
    enablePort(PORTF);
    _delay_cycles(3);

    // Configure LED and pushbutton pins
    selectPinPushPullOutput(RED_LED);
    selectPinPushPullOutput(GREEN_LED);
}
