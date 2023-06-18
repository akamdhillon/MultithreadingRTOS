/*
	kernel.c
	Created on: Jun. 15, 2023
	Author: akamdhillon

	The purpose of this file is to implement the kernel which is responsible
	for managing the system calls, initializing the kernel, starting the kernel,
	and task scheduling. This includes the core functionality of the OS.
*/



#include "kernel.h"
// Required for input/output capability in particular to printf().
#include<stdio.h>

//The current stack pointer for a thread. 32-bit unsigned int used for current
//but is then stored in threadArray[x].sp and is updated for next thread.
uint32_t* stackptr;
//Keeps track of last stack address used to ensure that there is no stack overflow.
uint32_t* lastStackAddress;

//Array of thread structs. It stores the metadata which in this case is the thread
//function and the stack pointer.
thread threadArray[maxStackSize/stackSize];

//Thread index number used to keep track of next available element to store metadata.
int threadIndex = 0;
//Keep track of the number of threads concurrently running.
int numThreadsRunning = 0;


//Handles supervisor calls. This means that the processor will 'jump' to the service
//call requested by the case number.
void SVC_Handler_Main(unsigned int *svc_args) {
	unsigned int svc_number;
	/*
	* Stack contains:
	* r0, r1, r2, r3, r12, r14, the return address and xPSR
	* First argument (r0) is svc_args[0]
	*/
	svc_number = ((char*)svc_args[6])[-2];
	switch(svc_number)
	{
		case 1:
			runFirstThread();
			break;
		case 2:
			//Pend an interrupt to do the context switch
			_ICSR |= 1<<28;
			__asm("isb");
			break;
		default: // unknown SVC
			break;
	}
}

//Creates a stack by allocating memory for a stack of 'stackSize'. Will return
//the new last stack address if there is space otherwise NULL.
uint32_t* createStack(void) {
	if((lastStackAddress - stackSize) < (uint32_t*)maxStackSize)
		return NULL;
	lastStackAddress = lastStackAddress - stackSize;
	return lastStackAddress;
}

//Sets up the thread by pushing values to registers from thread. Pushes xPSR value,
//pushes thread function, and lastly 0xA to 14 registers to fill all registers.
void setupThread(thread* tempThread) {
	*(--tempThread->sp) = 1<<24; //A magic number, this is xPSR
	*(--tempThread->sp) = (uint32_t)tempThread->thread_function;
	//Fill remaining 14 registers
	for(int x = 0; x < 14; x++)
	{
		*(--tempThread->sp) = 0xA;
	}
}

//Use createStack() to obtain stack pointer and ensure there is space in stack.
//Populate the thread array with the thread function input and setup the array.
int osCreateThread(void (*threadFunction)(void)) {
    stackptr = createStack();
    if (stackptr == NULL) {
    	printf("NULL PTR/r/n");
        return 0; // Stack allocation failed
    }
	threadArray[threadIndex].sp = stackptr;
	threadArray[threadIndex].thread_function = threadFunction;
	setupThread(&threadArray[threadIndex]);
	threadIndex++;
    return 1;
}

//Called first by user to initialize the global variables.
void osKernelInitialize() {
	lastStackAddress = *(uint32_t**)0x0;
	threadIndex = 0;
	//set the priority of PendSV to almost the weakest
	SHPR3 |= 0xFE << 16; //shift the constant 0xFE 16 bits to set PendSV priority
	SHPR2 |= 0xFDU << 24; //Set the priority of SVC higher than PendSV
}

//After initialization and creating threads, set PSP to the thread stack pointer
//and then make system call to runFirstThread function.
void osKernelStart() {
	__set_PSP((uint32_t)threadArray[threadIndex].sp);
	__asm("SVC #1");
}

//Complete the context switch between threads. Save the stack pointer of the current
//thread and then update the current thread to the next one and lastly set the PSP
//to the stack pointer of the next thread.
void osSched(void) {
	threadArray[threadIndex].sp = (uint32_t*)(__get_PSP() - 8*4);
	threadIndex = (threadIndex + 1) % numThreadsRunning;
	__set_PSP(threadArray[threadIndex].sp + 8*4);
}

//Makes a system call to pend an interrupt and make a context switch
void osYield(void) {
	__asm("SVC #2");
}
