// shell_interface.cpp
// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <shell_interface.h>

// Function to get Input from Terminal
void terminal::Terminal::getsUart0(USER_DATA* data)
{
    uint8_t count;
    char    c;

    count = data->characterCount;

    c = UART0_DR_R & 0xFF; // Get character

    UART0_ICR_R = 0xFFF; // Clear any UART0 interrupts

    if((c == 13) || (count == MAX_CHARS))
    {
        char buffer[10] = "\r\n";
        data->buffer[count++] = '\0';
        data->endOfString = true;
        sendUart0String(buffer);
    }
    else
    {
        if ((c == 8 || c == 127) && count > 0) // Decrement count if invalid character entered
        {
            count--;
            data->characterCount = count;
        }

        if (c >= ' ' && c < 127)
        {
            if('A' <= c && c <= 'Z')
            {
                data->buffer[count] = c + 32;  // Converts capital letter to lower case
            }
            else
            {
                data->buffer[count] = c;
            }
        }
    }
}

// Function to Tokenize Strings
void terminal::Terminal::parseFields(USER_DATA* data)
{
    char    c;
    uint8_t count, fieldIndex;

    count = data->characterCount;

    c = data->buffer[count];

    fieldIndex = data->fieldCount;

    if(c != '\0' && count <= MAX_CHARS)
    {
        if('a' <= c && c <= 'z') // Verify is character is an alpha (case sensitive)
        {
            if(data->delimeter == true) // Note: Leave if statement here so else statement is not entered
            {
                data->fieldPosition[fieldIndex] = count;
                data->fieldType[fieldIndex] = 'A';
                data->fieldCount = ++fieldIndex;
                data->delimeter = false;
            }
        }
        else if(('0' <= c && c <= '9') || ',' == c ||  c == '.') //Code executes for numerics same as alpha
        {
            if(data->delimeter == true) // Note: Leave if statement here so else statement is not entered
            {
                data->fieldPosition[fieldIndex] = count;
                data->fieldType[fieldIndex] = 'N';
                data->fieldCount = ++fieldIndex;
                data->delimeter = false;
            }
        }
        else // Insert NULL('\0') into character array if NON-alphanumeric character detected
        {
            data->buffer[count] = '\0';
            data->delimeter = true;
        }
        data->characterCount = ++count;
    }
}

// Function to Return a Token as a String
void terminal::Terminal::getFieldString(USER_DATA* data, char fieldString[], uint8_t fieldNumber)
{
    uint8_t offset, index = 0;

    offset = data->fieldPosition[fieldNumber]; // Get position of first character in string of interest

    while(data->buffer[offset] != '\0')
    {
        fieldString[index++] = data->buffer[offset++];
    }

    fieldString[index] = '\0';
}

// Function to Return a Token as an Integer
int32_t terminal::Terminal::getFieldInteger(USER_DATA* data, uint8_t fieldNumber)
{
    int32_t num;
    uint8_t offset, index = 0;
    char    copy[MAX_CHARS + 1];

    offset = data->fieldPosition[fieldNumber];  // Get position of first character in string of interest

    while(data->buffer[offset] != '\0')
    {
        copy[index++] = data->buffer[offset++];
    }

    copy[index] = '\0';

    num = atoi(copy);

    return num;
}

// Function Used to Determine if Correct Command Entered
bool terminal::Terminal::isCommand(USER_DATA* data, const char strCommand[], uint8_t minArguments)
{
    bool    command = false;
    uint8_t offset = 0, index = 0;
    char    copy[MAX_CHARS + 1];

    while(data->buffer[offset] != '\0')
    {
        copy[index++] = data->buffer[offset++];
    }

    copy[index] = '\0';

    if((strcmp(strCommand, copy) == 0) && (data->fieldCount >= minArguments))
    {
        command = true;
    }

    return command;
}

// Function to reset User Input for next command
void terminal::Terminal::resetUserInput(USER_DATA* data)
{
    data->characterCount = 0;
    data->fieldCount = 0;
    data->delimeter = true;
    data->endOfString = false;
}

