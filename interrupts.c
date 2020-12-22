// interrupts.c
// William Bozarth
// Created on: October 24, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "interrupts.h"
#include "uart0.h"


// REQUIRED: modify this function to add support for the system timer
// REQUIRED: in preemptive code, add code to request task switch
void systickIsr(void)
{
    NVIC_INT_CTRL_PENDSTSET; // SysTick Set Pending
    NVIC_INT_CTRL_PENDSTCLR; // SysTick Clear Pending
    NVIC_INT_CTRL_VEC_PEN_TICK; // SysTick
}

// REQUIRED: in coop and preemptive, modify this function to add support for task switching
// REQUIRED: process UNRUN and READY tasks differently
void pendSvIsr(void)
{
    NVIC_INT_CTRL_PEND_SV;   // PendSV Set Pending
    NVIC_INT_CTRL_UNPEND_SV; // PendSV Clear Pending
    NVIC_INT_CTRL_VEC_PEN_PNDSV; // PendSV
}

// REQUIRED: modify this function to add support for the service call
// REQUIRED: in preemptive code, add code to handle synchronization primitives
void svCallIsr(void)
{
    NVIC_INT_CTRL_VEC_PEN_SVC; // SVCall
}

// REQUIRED: code this function
void mpuFaultIsr(void)
{

}

// REQUIRED: code this function
void hardFaultIsr(void)
{
    char str[50];
    int pid = 0;

    sprintf(str, "  Hard fault in process %u\r\n", pid);
    sendUart0String(str);
}

// REQUIRED: code this function
void busFaultIsr(void)
{
    char str[50];
    int pid = 0;

    sprintf(str, "  Bus fault in process %u\r\n", pid);
    sendUart0String(str);
}

// REQUIRED: code this function
void usageFaultIsr(void)
{
    char str[50];
    int pid = 0;

    sprintf(str, "  Usage fault in process %u\r\n", pid);
    sendUart0String(str);
}
