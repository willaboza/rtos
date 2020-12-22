// subroutines.h

// William Bozarth
// Created on: December 21, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#ifndef SUBROUTINES_H_
#define SUBROUTINES_H_

#include "subroutines.h"

void initHw(void);
uint8_t readPbs(void);
void testLeds(void);
void testPbs(void);

#endif /* SUBROUTINES_H_ */
