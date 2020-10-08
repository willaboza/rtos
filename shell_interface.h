// shell_interface.h
// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#ifndef SHELL_INTERFACE_H_
#define SHELL_INTERFACE_H_

#ifdef __cplusplus

#include <cstdint>
#include <cstdbool>
#include <cstdlib>
#include <cstring>
#include "tm4c123gh6pm.h"
#include "uart0.h"

//
// Defines for Constants
//
#define MAX_CHARS 80
#define MAX_FIELDS 5

//
// Structure Definition
//
typedef struct _USER_DATA
{
    bool    delimeter;
    bool    endOfString;
    uint8_t fieldCount;
    uint8_t characterCount;
    uint8_t fieldPosition[MAX_FIELDS];
    char    fieldType[MAX_FIELDS];
    char    buffer[MAX_CHARS + 1];
} USER_DATA;

//
// Class Definition
//
namespace terminal
{
class Terminal
{
public:
    // Public Variables
    USER_DATA userInput;

    // Public Methods
    void getsUart0(USER_DATA* data);
    void parseFields(USER_DATA* data);
    void resetUserInput(USER_DATA* data);
    bool isCommand(USER_DATA* data, const char strCommand[], uint8_t minArguments);
    void getFieldString(USER_DATA* data, char fieldString[], uint8_t fieldNumber);
    int32_t getFieldInteger(USER_DATA* data, uint8_t fieldNumber);

    // Constructors
    Terminal() = default; // Zero-initialize object
    ~Terminal() = default;
};
}

#endif /* __cplusplus*/



#endif /* SHELL_INTERFACE_H_ */
