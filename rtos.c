// rtos.c
// William Bozarth
// Created on: October 21, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "rtos.h"
#include "uart0.h"

// Displays the process (thread) information
void ps(void)
{
    // Temporarily display text when function is called
    sendUart0String("  PS called\r\n");
}

// Displays the inter-process (thread) communication state
void ipcs(void)
{
    // Temporarily display text when function is called
    sendUart0String("  IPCS called\r\n");
}

// Kills the process (thread) with the matching PID
void kill(int pid)
{
    // Temporarily display text when function is called
    sendUart0String(" pid# killed\r\n");
}

// Turns priority inheritance on or off
void pi(bool on)
{
    if(on)
    {
        // Temporarily display text when function is called
        sendUart0String(" PI on\r\n");
    }
    else
    {
        // Temporarily display text when function is called
        sendUart0String(" PI off\r\n");
    }
}

// Turns preemption on or off
void preempt(bool on)
{
    if(on)
    {
        // Temporarily display text when function is called
        sendUart0String(" preempt on\r\n");
    }
    else
    {
        // Temporarily display text when function is called
        sendUart0String(" preempt off\r\n");
    }
}

// Selected priority or round-robin scheduling
void sched(bool prio_on)
{
    if(prio_on)
    {
        // Temporarily display text when function is called
        sendUart0String(" sched prio\r\n");
    }
    else
    {
        // Temporarily display text when function is called
        sendUart0String(" sched rr\r\n");
    }
}

// Displays the PID of the process (thread)
void pidof(char name[])
{
    // Temporarily display text when function is called
    sendUart0String(" proc_name launched\r\n");
}
