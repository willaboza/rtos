// uart0.h
// William Bozarth
// Created on: October 7, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#ifndef UART0_H_
#define UART0_H_

#ifndef UART0_HPP_
#define UART0_HPP_


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus*/


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"

//
// Pins and Defines
//
#define UART0_TX PORTA,1
#define UART0_RX PORTA,0
#define MAX_SUB_CHARS 80
#define MAX_TABLE_SIZE 10
#define QUEUE_BUFFER_LENGTH 250

//
// Structure Definition
//
typedef struct _Uart0_Buffer
{
    char uart0String[QUEUE_BUFFER_LENGTH];
    uint8_t writeIndex;
    uint8_t readIndex;
} Uart0_Buffer;

extern Uart0_Buffer uart0Info;

//
// Subroutines
//
void setUart0BaudRate(uint32_t baudRate, uint32_t fcyc);
void writeToQueue(char c);
bool kbhitUart0(void);
char getcUart0(void);
void initUart0(void);
char readFromQueue(void);
bool emptyRingBuffer(void);
bool fullRingBuffer(void);
void printMainMenu(void);
void printHelpInputs(void);
void printHelpOututs(void);
void sendUart0String(char str[]);
void sendUart0StringLiteral(const char str[]);
void newLine(void);
void uart0Isr(void);

#ifdef __cplusplus
}
#endif /* __cplusplus*/
#endif /* UART0_H_ */
