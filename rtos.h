// rtos.h
// William Bozarth
// Created on: October 21, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#ifndef RTOS_H_
#define RTOS_H_

#include "rtos.h"

void ps(void);
void ipcs(void);
void kill(int pid);
void pi(bool on);
void preempt(bool on);
void sched(bool prio_on);
void pidof(char name[]);

#endif /* RTOS_H_ */
