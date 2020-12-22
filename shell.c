// shell.c
// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "shell.h"
#include "uart0.h"
#include "reboot.h"
#include "rtos.h"
#include "gpio.h"

USER_DATA userInput = {.delimeter = true,
                       .endOfString = false,
                       .fieldCount = 0,
                       .startCount = 0,
                       .characterCount = 0,
};

// Function to get Input from Terminal
bool getsUart0(USER_DATA* data)
{
    char c;

    c = UART0_DR_R & 0xFF; // Get character

    UART0_ICR_R = 0xFFF; // Clear any UART0 interrupts

    // Determine if user input is complete
    if((c == 13) || ((data->characterCount + 1) % MAX_CHARS == data->startCount))
    {
        data->buffer[data->characterCount] = '\0';
        data->startCount = data->characterCount = (data->characterCount + 1) % MAX_CHARS;
        sendUart0String("\r\n");
        return true;
    }

    if(data->characterCount == data->startCount)
    {
        data->fieldCount = 0;
        data->delimeter = true;
    }

    if(c == 8 || c == 127) // Decrement count if invalid character entered
    {
        if(data->characterCount != data->startCount)
        {
            if(data->characterCount != 0)
                data->characterCount--;
            else
                data->characterCount = MAX_CHARS;

            // Removes character from terminal display
            sendUart0String("\b \b");
        }
    }
    else if(c >= ' ' && c < 127) // Converts capital letter to lower case (if necessary)
    {
        if('A' <= c && c <= 'Z')
        {
            data->buffer[data->characterCount] = c + 32;
            data->characterCount = (data->characterCount + 1) % MAX_CHARS;
        }
        else
        {
            data->buffer[data->characterCount] = c;
            data->characterCount = (data->characterCount + 1) % MAX_CHARS;
        }
    }

    return false;
}

// Function to Tokenize Strings
void parseFields(USER_DATA* data)
{
    char    c;
    uint8_t count, fieldIndex;

    count = data->characterCount - 1; // Subtract by one to get correct character count

    c = data->buffer[count];

    // Check if at end of user input and exit function if so,
    // or if backspace or delete was entered by user.
    if(c == '\0' || count > MAX_CHARS || c == 8 || c == 127)
        return;

    // Get current Index for field arrays
    fieldIndex = data->fieldCount;

    if('a' <= c && c <= 'z' || c == '/') // Verify is character is an alpha (case sensitive)
    {
        if(data->delimeter)
        {
            data->fieldPosition[fieldIndex] = count;
            data->fieldType[fieldIndex] = 'A';
            data->fieldCount += 1;
            data->delimeter = false;
        }
    }
    else if('0' <= c && c <= '9') //Code executes for numerics same as alpha
    {
        if(data->delimeter)
        {
            data->fieldPosition[fieldIndex] = count;
            data->fieldType[fieldIndex] = 'N';
            data->fieldCount += 1;
            data->delimeter = false;
        }
    }
    else // Insert NULL('\0') into character array if NON-alphanumeric character detected
    {
        data->buffer[count] = '\0';
        data->delimeter = true;
    }
}

// Function to Return a Token as a String
void getFieldString(USER_DATA** data, char fieldString[], uint8_t fieldNumber)
{
    uint8_t offset, index = 0;

    // Get position of first character in string of interest
    offset = (*data)->fieldPosition[fieldNumber];

    // Copy characters for return
    while((*data)->buffer[offset] != '\0')
    {
        fieldString[index++] = (*data)->buffer[offset++];
        offset %= MAX_CHARS;
    }

    // Add NULL to terminate string
    fieldString[index] = '\0';
}

// Function to Return a Token as an Integer
int32_t getFieldInteger(USER_DATA** data, uint8_t fieldNumber)
{
    uint8_t offset, index = 0;
    char copy[MAX_CHARS];

    offset = (*data)->fieldPosition[fieldNumber];  // Get position of first character in string of interest

    while((*data)->buffer[offset] != '\0')
    {
        copy[index++] = (*data)->buffer[offset++];
        offset %= MAX_CHARS;
    }

    copy[index] = '\0';

    return atoi(copy);
}


// Function Used to Determine if Correct Command Entered
bool isCommand(USER_DATA** data, const char strCommand[], uint8_t minArguments)
{
    bool ok = false;
    int val;
    uint8_t c1, c2, offset, index = 0;

    if((*data)->fieldCount < minArguments)
        return false;

    offset = (*data)->fieldPosition[0];

    while((c1 = strCommand[index++]) != '\0')
    {
        c2 = (*data)->buffer[offset++];
        val = c1 - c2;

        if(val != 0 || c2 == 0)
            return false;

        offset %= MAX_CHARS;
        ok = true;
    }

    return ok;
}

void shellCommands(USER_DATA* userInput)
{
    char buffer[MAX_CHARS + 1];

    while(true)
    {
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
