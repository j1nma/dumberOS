# OS Kernel Construction

This project consists of building a simple kernel based on a separate project about computer architecture. IPC, Memory Management, Scheduling and Memory Protection will be implemented.

## Previous requirements

A functional computer architecture project, that is, a 64-bit monolithic kernel with basic interruption management, system calls, keyboard driver, video driver in text mode and Kernel and User Spaces separated, where device drivers can be added to the kernel as modules.

Docker is used as host development environment.

## Features

### System calls

System calls are the interface between user and kernel space. The system must provide system calls for the processes (explained later) to interact with the kernel. The same mechanism developed in the Computer Architecture project (software interruptions) is used.

### Physical Memory Management

The kernel must have some simple system to reserve and release pages of at least 4 contiguos KB (page allocator). Note that this is not tied to virtual memory / pagination: the requirement is only to reserve and release blocks of physical memory. This memory can be used by the kernel for its internal structures, or a process in user space.

Syscalls involved
- Reserve memory for the calling process
- Free memory of the calling process

### Processes, Threads, Context Switching y Scheduling

The system must have pre-emptive multitasking in a variable amount of processes. To do this, the system must implement some mechanism that allows to suspend the execution of a process and continue the execution of another one (context switching), without requiring that they release the control of the CPU, and with some structure / algorithm that allows to select the next process (scheduler).

It must also have threads implementation, with its own scheduler within each process.

Syscalls involved
- Create, Finish, and List Processes
- Create, Finish, and List threads

Optional (can be used to implement IPCs later)
- Sleep the current process until an x event occurs.
- Send a signal to a process x (awakens it if it is asleep)
- Give the processor the following process (yield)

### IPCs

Sending and receiving blocking messages must be implemented, of a fixed number of bytes, sent to a common identifier (string of characters) defined between the processes to be communicated (a combination of PIDs, a domain name, or something else).

Mutexes are also implemented: they can execute their up and down operations on an agreed identifier.

Syscalls involved
Send and Receive for messages, being the latter blocking. Up and Down for mutexes. The scheduler design should contemplate these syscalls. A thread waiting for a message or mutex should not block the other threads.

### Drivers

Two drivers: keyboard and video in text mode.

Keyboard
Use the keyboard driver implemented in the previous PW. Add the necessary syscalls so that a program can react immediately upon pressing a key (for example, a blocking system call that returns after a key is pressed without consuming it from the original keyboard buffer).

Video in text mode
Use the video driver implemented in the previous PW.


## Authors

* **Juan Manuel Alonso** - (https://github.com/j1nma)
* **Ioni Zelig Nielavitzky** - (https://github.com/jnielavitzky)
* **Nicolás Paganini** - (https://github.com/npaganini)
* **Martina Scomazzon** - (https://github.com/mscomazzon)

## License

MIT License

Copyright (c) [2017] [Alonso, Juan Manuel; Nielavitzky, Ioni Zelig; Scomazzon, Martina; Paganini, Nicolás]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
