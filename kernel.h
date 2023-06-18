/*
	kernel.h
	Created on: Jun. 15, 2023
	Author: akamdhillon

	Contains all the structs, functions that kernel.c uses along with other
	definitions. These include extern functions, #define, and constant definitions

 */

//calling extern function from another file instead of including entire file.
extern void runFirstThread(void);

//Standard c files include this, acts as guard and defines header file.
#ifndef INC_KERNEL_H_
#define INC_KERNEL_H_

//Set SVC priority high than PendSv by setting two registers
#define SHPR2 *(uint32_t*)0xE000ED1C //for setting SVC priority, bits 31-24
#define SHPR3 *(uint32_t*)0xE000ED20 // PendSV is bits 23-16
#define _ICSR *(uint32_t*)0xE000ED04 //This lets us trigger PendSV

//Include libraries and headers required
#include<stdio.h>
#include<stdint.h>
#include"stm32f4xx_hal.h"

//Define constants
#define stackSize 0x200
#define maxStackSize 0x4000

//A struct for each thread that contains metadata including the stack pointer and
//the function pointer.
typedef struct k_thread{
	uint32_t* sp; //stack pointer
	void (*thread_function)(void*); //function pointer
}thread;

//Function declarations.
void SVC_Handler_Main(unsigned int *svc_args);
uint32_t* createStack(void);
void setupThread(thread* tempThread);
int osCreateThread(void (*threadFunction)(void));
void osKernelInitialize();
void osKernelStart();
void osSched(void);
void osYield(void);
void threadFunction1();
void threadFunction2();

#endif /* INC_KERNEL_H_ */
