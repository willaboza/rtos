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
#include "tasks.h"
#include "subroutines.h"


//
// Start of Main Function
//
int main(void)
{

    // Declare variables
    bool ok;

    // Initialize hardware
    initHw();
    initUart0(115200, 40e6);
    initRtos();
    // initWatchdog();

    // Test blue LED is functioning correctly (turn on and then off)
    setPinValue(GREEN_LED_ONBOARD, 1);
    waitMicrosecond(250000);
    setPinValue(GREEN_LED_ONBOARD, 0);
    waitMicrosecond(250000);

    // testPbs(); // Used for troubleshooting LEDs and PB's

    //Print Main Menu
    printMainMenu();

    // Initialize semaphores
    keyPressed = createSemaphore(1);
    keyReleased = createSemaphore(0);
    flashReq = createSemaphore(5);
    resource = createSemaphore(1);

    // Add required idle process at lowest priority
    ok =  createThread(idle, "Idle", 15, 1024);

    // Add other processes
    ok &= createThread(lengthyFn,     "LengthyFn", 12, 1024);
    ok &= createThread(flash4Hz,      "Flash4Hz",  8,  1024);
    ok &= createThread(oneshot,       "OneShot",   4,  1024);
    ok &= createThread(readKeys,      "ReadKeys",  12, 1024);
    ok &= createThread(debounce,      "Debounce",  12, 1024);
    ok &= createThread(important,     "Important", 0,  1024);
    ok &= createThread(uncooperative, "Uncoop",    10, 1024);
    ok &= createThread(errant,        "Errant",    8,  1024);
    ok &= createThread(shell,         "Shell",     8,  1024);

    // Start up RTOS
    if (ok)
        startRtos(); // never returns
    else
        setPinValue(RED_LED, 1);

    return 0;
}
