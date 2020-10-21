// Watchdog Example
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz


// Watchdog counter 0:
//   This counter is driven by system clock

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#ifndef REBOOT_H_
#define REBOOT_H_

#include "reboot.h"

#define TIMEOUT_MS 2000 // Watchdog timeout

extern bool rebootFlag;

void initWatchdog(void);
void resetWatchdog(void);
void watchdogIsr(void);

#endif /* REBOOT_H_ */
