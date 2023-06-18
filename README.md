# MultithreadingRTOS

This project involves an efficient operating system that supports the concurrent execution of multiple threads on the STM32F4 microcontroller. This study of embedded systems is focused on the ARM Cortex-M4 architecture.

Key Project Objectives:

1. Design and implement a cooperative multitasking OS for the ARM Cortex-M4 microcontroller.
2. Develop functions for creating and managing threads, including stack allocation and thread metadata.
3. Implement context switching to facilitate efficient thread execution and resource allocation.
4. Incorporate a yield system call to enable cooperative scheduling, allowing threads to voluntarily release the processor.

The core files are uploaded to the repo. The files 'kernel.h' and 'kernel.c' were written by me and are called in main.c to allow for concurrently running the functions; threadFunction1() and threadFunction2().

This project focuses on the following skill development:

1. Embedded systems programming for the ARM Cortex-M4 microcontroller.
2. Real-time operating system concepts and implementation.
3. Proficiency in C/C++ programming for microcontrollers.
4. Context switching and multitasking techniques.
5. Memory management and stack allocation in an embedded system.
6. Debugging and testing of microcontroller-based systems.
