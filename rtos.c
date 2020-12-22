// rtos.c
// William Bozarth
// Created on: October 21, 2020

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "rtos.h"
#include "uart0.h"

semaphore semaphores[MAX_SEMAPHORES] = {0};
uint8_t semaphoreCount = 0;
uint8_t keyPressed = 0;
uint8_t keyReleased = 0;
uint8_t flashReq = 0;
uint8_t resource = 0;
uint8_t taskCurrent = 0;    // index of last dispatched task
uint8_t taskCount = 0;      // total number of valid tasks

//
// Kernel Functions
//

// REQUIRED: initialize systick for 1ms system timer
void initRtos(void)
{
    uint8_t i;
    // no tasks running
    taskCount = 0;
    // clear out tcb records
    for (i = 0; i < MAX_TASKS; i++)
    {
        tcb[i].state = STATE_INVALID;
        tcb[i].pid = 0;
    }
}

// REQUIRED: Implement prioritization to 16 levels
int rtosScheduler(void)
{
    bool ok;
    static uint8_t task = 0xFF;
    ok = false;
    while (!ok)
    {
        task++;
        if (task >= MAX_TASKS)
            task = 0;
        ok = (tcb[task].state == STATE_READY || tcb[task].state == STATE_UNRUN);
    }
    return task;
}

//
bool createThread(_fn fn, const char name[], uint8_t priority, uint32_t stackBytes)
{
    bool ok = false;
    uint8_t i = 0;
    bool found = false;
    // REQUIRED: store the thread name
    // add task if room in task list
    // allocate stack space for a thread and assign to sp below
    if (taskCount < MAX_TASKS)
    {
        // make sure fn not already in list (prevent reentrancy)
        while (!found && (i < MAX_TASKS))
        {
            found = (tcb[i++].pid ==  fn);
        }
        if (!found)
        {
            // find first available tcb record
            i = 0;
            while (tcb[i].state != STATE_INVALID) {i++;}
            tcb[i].state = STATE_UNRUN;
            tcb[i].pid = fn;
            tcb[i].sp = 0;
            tcb[i].priority = priority;
            tcb[i].currentPriority = priority;
            // increment task count
            taskCount++;
            ok = true;
        }
    }
    // REQUIRED: allow tasks switches again
    return ok;
}

// REQUIRED: modify this function to restart a thread
void restartThread(_fn fn)
{

}

// REQUIRED: modify this function to destroy a thread
// REQUIRED: remove any pending semaphore waiting
// NOTE: see notes in class for strategies on whether stack is freed or not
void destroyThread(_fn fn)
{

}

// REQUIRED: modify this function to set a thread priority
void setThreadPriority(_fn fn, uint8_t priority)
{

}

//
int8_t createSemaphore(uint8_t count)
{
    int8_t index = -1;
    if (semaphoreCount < MAX_SEMAPHORES)
    {
        semaphores[semaphoreCount].count = count;
        index = semaphoreCount;
        semaphoreCount++;
    }
    return index;
}

// REQUIRED: modify this function to start the operating system, using all created tasks
void startRtos(void)
{

}

// REQUIRED: modify this function to yield execution back to scheduler using pendsv
// push registers, call scheduler, pop registers, return to new function
void yield(void)
{

}

// REQUIRED: modify this function to support 1ms system timer
// execution yielded back to scheduler until time elapses using pendsv
// push registers, set state to delayed, store timeout, call scheduler, pop registers,
// return to new function (separate unrun or ready processing)
void sleep(uint32_t tick)
{

}

// REQUIRED: modify this function to wait a semaphore with priority inheritance
// return if avail (separate unrun or ready processing), else yield to scheduler using pendsv
void wait(int8_t semaphore)
{

}

// REQUIRED: modify this function to signal a semaphore is available using pendsv
void post(int8_t semaphore)
{

}

// Displays the process (thread) information
void ps(void)
{
    // Temporarily display text when function is called
    sendUart0String("  PS called\r\n");
}

// Displays the inter-process (thread) communication state
void ipcs(void)
{
    // Temporarily display text when function is called
    sendUart0String("  IPCS called\r\n");
}

// Kills the process (thread) with the matching PID
void kill(int pid)
{
    // Temporarily display text when function is called
    sendUart0String(" pid# killed\r\n");
}

// Turns priority inheritance on or off
void pi(bool on)
{
    if(on)
    {
        // Temporarily display text when function is called
        sendUart0String(" PI on\r\n");
    }
    else
    {
        // Temporarily display text when function is called
        sendUart0String(" PI off\r\n");
    }
}

// Turns preemption on or off
void preempt(bool on)
{
    if(on)
    {
        // Temporarily display text when function is called
        sendUart0String(" preempt on\r\n");
    }
    else
    {
        // Temporarily display text when function is called
        sendUart0String(" preempt off\r\n");
    }
}

// Selected priority or round-robin scheduling
void sched(bool prio_on)
{
    if(prio_on)
    {
        // Temporarily display text when function is called
        sendUart0String(" sched prio\r\n");
    }
    else
    {
        // Temporarily display text when function is called
        sendUart0String(" sched rr\r\n");
    }
}

// Displays the PID of the process (thread)
void pidof(char name[])
{
    // Temporarily display text when function is called
    sendUart0String(" proc_name launched\r\n");
}
