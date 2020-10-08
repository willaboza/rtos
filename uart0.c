// uart0.cpp
// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include "uart0.h"

Uart0_Buffer uart0Info = {0};

// Initialize UART0
void initUart0(void)
{
    // Enable clocks
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    _delay_cycles(3);

    enablePort(PORTA);
    _delay_cycles(3);

    // Configure UART0 pins
    selectPinPushPullOutput(UART0_TX);
    selectPinDigitalInput(UART0_RX);
    setPinAuxFunction(UART0_TX, GPIO_PCTL_PA1_U0TX);
    setPinAuxFunction(UART0_RX, GPIO_PCTL_PA0_U0RX);

    // Configure UART0 with default baud rate
    UART0_CTL_R = 0;                                    // turn-off UART0 to allow safe programming
    UART0_CC_R  = UART_CC_CS_SYSCLK;                     // use system clock (usually 40 MHz)
}

// Set baud rate as function of instruction cycle frequency
void setUart0BaudRate(uint32_t baudRate, uint32_t fcyc)
{
    uint32_t divisorTimes128 = (fcyc * 8) / baudRate;   // calculate divisor (r) in units of 1/128,
                                                        // where r = fcyc / 16 * baudRate
    UART0_CTL_R  = 0;                                   // turn-off UART0 to allow safe programming
    UART0_IBRD_R = divisorTimes128 >> 7;                // set integer value to floor(r)
    UART0_FBRD_R = ((divisorTimes128 + 1) >> 1) & 63;   // set fractional value to round(fract(r)*64)
    UART0_LCRH_R = UART_LCRH_WLEN_8;                    // configure for 8N1 w/ 16-level FIFO
    UART0_CTL_R  = UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN; // turn-on UART0
    UART0_IM_R = UART_IM_TXIM;                          // turn-on TX interrupt
    NVIC_EN0_R |= 1 << (INT_UART0-16);                  // turn-on interrupt 21 (UART0)
}


// Returns the status of the receive buffer
bool kbhitUart0(void)
{
    return !(UART0_FR_R & UART_FR_RXFE);
}

// Blocking function that returns with serial data once the buffer is not empty
char getcUart0(void)
{
    return UART0_DR_R & 0xFF; // get character from fifo
}

// Add characters to UART0 TX FIFO
void sendUart0String(char str[])
{
    int i = 0;

    // Write string to Tx Ring Buffer
    while(str[i] != '\0')
    {
        // If NOT full write next Character
        if(!(fullRingBuffer()))
        {
            writeToQueue(str[i]);
            i++;
        }
    }

    UART0_ICR_R = 0x00000FFF;
    // Check to see if UART Tx holding register is empty
    if(UART0_FR_R & UART_FR_TXFE  && !(emptyRingBuffer()))
    {
        UART0_DR_R = readFromQueue(); // "Prime Pump" by writing 1st char to Uart0
    }
}

// Add characters to UART0 TX FIFO
void sendUart0StringLiteral(const char str[])
{
    int i = 0;

    // Write string to Tx Ring Buffer
    while(str[i] != '\0')
    {
        // If NOT full write next Character
        if(!(fullRingBuffer()))
        {
            writeToQueue(str[i]);
            i++;
        }
    }

    UART0_ICR_R = 0x00000FFF;
    // Check to see if UART Tx holding register is empty
    if(UART0_FR_R & UART_FR_TXFE  && !(emptyRingBuffer()))
    {
        UART0_DR_R = readFromQueue(); // "Prime Pump" by writing 1st char to Uart0
    }
}

// Update current position of writeIndex variable for Ring Buffer
void writeToQueue(char c)
{
    uart0Info.uart0String[uart0Info.writeIndex] = c;
    uart0Info.writeIndex = (uart0Info.writeIndex + 1) % QUEUE_BUFFER_LENGTH;
}

// Update current position of readIndex variable for Ring Buffer
char readFromQueue(void)
{
    char c;
    c = uart0Info.uart0String[uart0Info.readIndex];
    uart0Info.readIndex = (uart0Info.readIndex + 1) % QUEUE_BUFFER_LENGTH;
    return c;
}

// Returns true if ring buffer is EMPTY
bool emptyRingBuffer(void)
{
    bool ok = false;

    if(uart0Info.writeIndex == uart0Info.readIndex)
    {
        ok = true;
    }

    return ok;
}

// Returns true if ring buffer is FULL
bool fullRingBuffer(void)
{
    bool ok = false;

    if(((uart0Info.writeIndex + 1) % QUEUE_BUFFER_LENGTH) == uart0Info.readIndex)
    {
        ok = true;
    }

    return ok;
}

// Function to Print Main Menu
void printMainMenu(void)
{
    char buffer[QUEUE_BUFFER_LENGTH] = "\r\n"
            "Commands:\r\n"
            "  dhcp ON|OFF|REFRESH|RELEASE\r\n"
            "  set IP|GW|DNS|SN|MQTT w.x.y.z\r\n"
            "  ifconfig\r\n"
            "  publish TOPIC DATA\r\n"
            "  subscribe TOPIC\r\n"
            "  unsubscribe TOPIC\r\n"
            "  connect\r\n"
            "  disconnect\r\n"
            "  help Inputs\r\n"
            "  help Outputs\r\n"
            "  help Subs\r\n"
            "  reboot\r\n\r\n";

    sendUart0String(buffer); // Place contents of menu into queue
}


// Function to Print Help Inputs
void printHelpInputs(void)
{
    char buffer[QUEUE_BUFFER_LENGTH] = "\r\n"
            "  Local Input Topics to MQTT Client:\r\n"
            "    env/pb\r\n"
            "    env/temp\r\n"
            "    env/led/green on|off\r\n"
            "    env/led/red   on|off\r\n"
            "    env/led/blue  on|off\r\n\r\n";

    sendUart0String(buffer); // Place contents of menu into queue
}

// Function to Print Help Outputs
void printHelpOututs(void)
{
    char buffer[QUEUE_BUFFER_LENGTH] = "\r\n"
            "  Local Output Topics to MQTT Client:\r\n"
            "    env/uart\r\n"
            "    env/led/green STATUS\r\n"
            "    env/led/red   STATUS\r\n"
            "    env/led/blue  STATUS\r\n\r\n";

    sendUart0String(buffer); // Place contents of menu into queue
}

// Handle UART0 Interrupts
void uart0Isr(void)
{
    // Writing a 1 to the bits in this register clears the bits in the UARTRIS and UARTMIS registers
    UART0_ICR_R = 0xFFF;
    // Check to see if UART Tx holding register is empty and send next byte of data
    if((UART0_FR_R & UART_FR_TXFE) && !(emptyRingBuffer()))
    {
        UART0_DR_R = readFromQueue();
    }
}



