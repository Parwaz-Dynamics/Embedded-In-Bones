# Embedded-In-Bones 🦴

**My personal journey into embedded systems, from bare-metal programming to real-time operating systems.**

This repository documents my hands-on learning journey in embedded systems. It contains experiments, exercises, and projects as I work toward understanding what happens inside a microcontroller — from executing individual instructions to building real-time, event-driven applications.

## 🎯 Why This Repository?

I've worked with Arduino, ESP32, and Teensy platforms, along with STM32 microcontrollers such as the STM32F103, STM32F411, and STM32H723. Although I had some practical experience, I realized there was still a lot I wanted to understand at a deeper level.

Instead of relying entirely on frameworks and libraries, I decided to go back to the fundamentals: learning how processors execute instructions, how memory works, how interrupts are handled, and how embedded software can be designed to operate reliably in real time.

This repository is a record of that journey — including the concepts I've learned, the challenges I've faced, and the things I still need to explore.

## 📂 Learning Journey

### 1. Bare-Metal STM32F446

**Learning resource:** *Embedded Systems with ARM Cortex-M Microcontrollers in Assembly Language and C* by Dr. Yifeng Zhu.

I started by learning how to program a microcontroller close to the hardware, using ARM assembly language and C.

Topics covered include:

- ARM Cortex-M instruction set and processor registers
- Arithmetic, logical, shifting, and branching instructions
- Subroutines, structured programming, and mixing assembly with C
- Stack and heap fundamentals
- Static and volatile variables
- GPIO configuration and digital I/O
- Interrupts and timers
- Basic communication protocols
- Memory organization, including Flash and RAM
- Debugging and the software build process

This book helped me develop a clearer understanding of what happens inside a microcontroller when a program runs, rather than treating the hardware as a black box.

Along the way, I also gained practical experience with **Keil µVision**, ARM Compiler 6, and the GNU toolchain. I explored debugging, ELF files, linker scripts, and how code and data are organized in memory.

The final DSP chapter is still on my list to revisit.

📁 **Folder:** `Bare-Metal STM32F446/`

### 2. Real-Time Operating Systems — Quantum Leaps

**Learning resource:** [Quantum Leaps — YouTube lectures](https://www.youtube.com/@StateMachineCOM)

After completing most of the bare-metal material, I wanted to understand how embedded applications manage multiple concurrent activities and respond to events in real time.

I started following the Modern Embedded Systems programming lectures by Quantum Leaps. The early material reinforced some concepts I had already studied, but things became more challenging as the lectures progressed into interrupt handling, concurrency, and real-time operating systems.

During this stage, I worked through **MiROS**, a small real-time operating system, and then moved on to **QPC (QP/C)**.

Building and studying MiROS helped me explore concepts such as:

- Tasks and cooperative scheduling
- Preemptive scheduling and context switching
- Interrupt handling and synchronization
- Semaphores and blocking
- Communication and synchronization between concurrent tasks

My journey with QPC is ongoing. Next, I plan to explore state machines and event-driven programming to better understand how embedded applications can be structured without relying entirely on traditional blocking task-based designs.

📁 **Folder:** `Quantum Leap/`

## 🛠️ Tools & Technologies

- **Microcontrollers:** STM32F446 and other STM32 platforms
- **Programming languages:** C and ARM Assembly
- **IDE:** Keil µVision
- **Toolchains:** ARM Compiler 6 and GNU toolchain
- **Core topics:** Bare-metal programming, interrupts, timers, memory management, debugging, and RTOS fundamentals
- **RTOS / frameworks:** MiROS and QP/C

## 🚧 What's Next?

This journey is far from over.

This list will evolve as I learn and discover new areas worth exploring.

## 📌 A Note to Myself

Keep it up

------------------------------------------------

## 🔄 A Change in Direction: PCB Design & Embedded Systems Engineering

For now, I'm taking a slight detour from RTOS development to explore another important side of embedded systems: **PCB design and hardware development**.

I want to move beyond working with development boards and start understanding how to design and build my own PCBs. My plan is to learn the fundamentals of PCB design and manufacture one or two boards of my own.

Alongside this, I'll be reading *Making Embedded Systems* by **Elecia White**. I see this book as an opportunity to learn more about the broader engineering side of embedded systems, including how embedded projects are designed, developed, debugged, and managed.

My current plan is to:

- Learn the fundamentals of PCB design.
- Design and build one or two PCBs to gain practical experience.
- Read *Making Embedded Systems* by Elecia White.
- Return to Miro Samek's Modern Embedded Systems lectures after establishing a basic understanding of PCB design and completing my first boards.
- Meanwhile work on another project where I can practice the already learned techniques.

I'll resume the lectures from **Lecture #29 — Object-Oriented Programming**, where I paused.
