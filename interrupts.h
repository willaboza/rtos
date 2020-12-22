// interrupts.h
// William Bozarth
// Created on: October 24, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "interrupts.h"

#define HANDLER_MODE 0xFFFFFFE1;
#define THREAD_MODE  0xFFFFFFE9;

void systickIsr(void);
void pendSvIsr(void);
void svCallIsr(void);
void mpuFaultIsr(void);
void hardFaultIsr(void);
void busFaultIsr(void);
void usageFaultIsr(void);

#endif /* INTERRUPTS_H_ */
