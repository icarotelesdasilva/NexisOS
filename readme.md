# NexisOS

<p align="center">
  <strong>Practical Operating System Built Around the NexisK Kernel</strong>
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#current-status">Current Status</a> •
  <a href="#features">Features</a> •
  <a href="#roadmap">Roadmap</a> •
  <a href="#building">Building</a>
</p>

## Overview

**NexisOS** is the practical operating system built around the **NexisK kernel**.

The purpose of this project is to take the kernel concepts developed in NexisK and put them into practice as an actual operating system environment.

Instead of testing kernel subsystems only in isolation, NexisOS integrates them into a single bootable system where the kernel, bootloader, memory management, interrupts, hardware drivers, input handling, and user-facing interfaces work together.

NexisOS is therefore the practical execution environment for NexisK.

> **NexisK develops the kernel. NexisOS puts the kernel into practice.**

The project is developed from low-level fundamentals, with direct interaction with x86 hardware and minimal reliance on external operating-system infrastructure.

## Goals

NexisOS focuses on learning and implementing the fundamental components required to build an operating system from the ground up.

The main goals are:

* Put NexisK into practice as a real operating system.
* Integrate kernel subsystems into a functional environment.
* Boot directly through the Nexis boot chain.
* Manage physical memory.
* Handle hardware interrupts.
* Communicate with hardware devices.
* Provide keyboard input.
* Provide a basic terminal environment.
* Experiment with processes and scheduling.
* Test the system on real x86 hardware.
* Keep the implementation understandable and close to the hardware.

NexisOS is not intended to reproduce an existing operating system. It is an independent systems programming project.

## Architecture

NexisOS is built around several independent components that work together.

```text
                    ┌──────────────────────┐
                    │       NexisOS        │
                    │   Operating System   │
                    └──────────┬───────────┘
                               │
                    ┌──────────▼───────────┐
                    │       NexisK         │
                    │        Kernel        │
                    └──────────┬───────────┘
                               │
        ┌──────────────────────┼──────────────────────┐
        │                      │                      │
   Memory Management       Interrupts             Drivers
        │                      │                      │
       PMM                  IDT/PIC              VGA/Serial
        │                      │                  Keyboard
        │                      │                   Mouse
        └──────────────────────┼──────────────────────┘
                               │
                    ┌──────────▼───────────┐
                    │    NexisLoader       │
                    │      Bootloader       │
                    └──────────┬───────────┘
                               │
                    ┌──────────▼───────────┐
                    │      x86 Hardware     │
                    └──────────────────────┘
```

The architecture is intentionally kept modular so individual subsystems can be developed and tested independently before being integrated into the operating system.

## NexisK

NexisK is the kernel at the center of NexisOS.

It provides the low-level execution environment and contains the core operating-system mechanisms.

NexisK currently targets **32-bit x86 (i386)** and is developed primarily using:

* C
* NASM assembly
* freestanding development
* direct hardware access

The kernel contains components such as:

* Global Descriptor Table (GDT)
* Interrupt Descriptor Table (IDT)
* Programmable Interrupt Controller (PIC)
* Programmable Interval Timer (PIT)
* Physical Memory Manager (PMM)
* Keyboard handling
* PS/2 mouse handling
* VGA text output
* Serial output
* System call infrastructure
* Initial process infrastructure

NexisOS provides the environment where these components are integrated and exercised together.

## NexisLoader

NexisOS uses the **NexisLoader** bootloader as part of its boot process.

NexisLoader is an independent x86 BIOS bootloader developed specifically for the Nexis ecosystem.

Its responsibilities include:

* Initial bootstrapping
* Kernel loading
* Kernel selection
* Memory map detection
* Passing hardware information to the kernel
* Preparing the environment required by NexisK

The bootloader and kernel are intentionally separated into independent projects.

This allows NexisK to remain focused on kernel development while NexisLoader handles the early boot environment.

## Memory Management

NexisOS uses the memory information provided by the bootloader to initialize the kernel's physical memory management.

The Physical Memory Manager currently works with the system's **E820 memory map**.

Usable memory regions are identified and divided into **4 KiB pages**.

The current implementation uses a bitmap representation where each physical page is represented by one byte.

```text
E820 memory map
       │
       ▼
Identify usable regions
       │
       ▼
Calculate number of pages
       │
       ▼
Calculate physical addresses
       │
       ▼
Calculate bitmap indices
       │
       ▼
Track usable physical pages
```

The PMM is still under development and will eventually provide complete page allocation, freeing, and memory reservation mechanisms.

## Interrupts

Hardware interrupts are an essential part of NexisOS.

The kernel uses:

* IDT
* PIC
* IRQ handlers
* Assembly interrupt entry points
* C interrupt handlers

For example, the keyboard is connected through **IRQ1**, which is mapped through the PIC into the kernel's interrupt vector space.

The general flow is:

```text
Hardware
   │
   ▼
IRQ
   │
   ▼
PIC
   │
   ▼
IDT
   │
   ▼
Assembly ISR
   │
   ▼
C Handler
   │
   ▼
Kernel subsystem
```

This structure allows hardware events to enter the kernel through a controlled interrupt path.

## Keyboard and Shell

NexisOS includes a basic keyboard input system using the PS/2 keyboard interface.

Keyboard scancodes are translated into characters and passed to a small shell implementation.

The current shell provides a basic command-line interface:

```text
NexisK> help

Available commands:
  help
  clear
  version
```

The shell currently supports:

* Character input
* Numbers
* Spaces
* Enter
* Backspace
* Basic command parsing
* Basic commands

The shell is intentionally simple at this stage.

It exists primarily as a practical interface for interacting with the kernel while the operating system is being developed.

## VGA Terminal

NexisOS currently uses VGA text mode for basic terminal output.

The VGA driver provides:

* Text output
* Screen clearing
* Cursor position tracking
* Backspace handling
* Newline handling
* Direct access to VGA memory

The terminal cursor is maintained independently by the VGA driver.

This allows higher-level components such as the shell to request operations without directly manipulating VGA memory.

## Current Status

NexisOS is currently in **early operating-system development**.

The project has moved beyond simply creating isolated kernel components and is now focused on integrating those components into a functioning operating-system environment.

### Currently implemented

* [x] x86 kernel boot
* [x] Protected mode
* [x] GDT
* [x] IDT
* [x] PIC
* [x] PIT
* [x] Serial output
* [x] VGA text output
* [x] Keyboard IRQ
* [x] Keyboard input
* [x] Basic shell
* [x] Shell command parsing
* [x] Backspace
* [x] Initial PMM
* [x] E820 memory map integration
* [x] Physical page tracking
* [ ] Complete physical page allocation
* [ ] Physical page freeing
* [ ] Complete memory reservation
* [ ] Virtual memory
* [ ] Process execution
* [ ] Scheduler
* [ ] Filesystem
* [ ] User-space programs

## Features

### Kernel

* 32-bit x86 kernel
* Freestanding C environment
* NASM assembly
* Protected mode execution
* GDT
* IDT
* Interrupt handling
* PIC
* PIT
* System call infrastructure
* Initial process infrastructure

### Memory

* E820 memory map
* Physical memory manager
* 4 KiB page tracking
* Bitmap-based physical page representation
* Usable memory detection

### Hardware

* VGA text mode
* Serial port
* PS/2 keyboard
* PS/2 mouse
* Programmable interrupt controller
* Programmable interval timer

### User Interface

* VGA terminal
* Keyboard input
* Shell prompt
* Command parsing
* `help`
* `clear`
* `version`
* Backspace support

## Development Philosophy

NexisOS follows a simple principle:

> **Build the system from the lowest practical level upward.**

The project avoids depending on a large external kernel framework.

Subsystems are implemented directly and then integrated into the operating system.

This makes failures useful: when something breaks, the goal is to understand which layer failed rather than hide the problem behind a high-level abstraction.

The project is also used as a practical environment for validating NexisK development.

A subsystem that works independently is one thing.

A subsystem that survives integration with the bootloader, kernel, interrupt system, memory manager, drivers, and shell is a much stronger test.

## Testing

NexisOS is intended to be tested in multiple environments.

### Emulation and Virtual Machines

Development can be performed using x86 virtualization and emulation environments.

These environments are useful for:

* Rapid testing
* Debugging
* Reproducible failures
* Development without physical hardware

### Real Hardware

Real x86 hardware is an important part of the project.

Hardware testing exposes problems that may not appear under emulation, including:

* Firmware differences
* Hardware initialization differences
* Timing differences
* Memory layout differences
* Device-specific behavior

NexisOS therefore aims to eventually validate its components through actual hardware execution rather than relying exclusively on virtual machines.

## Build

NexisOS uses a freestanding x86 build environment.

Typical components include:

* GCC
* NASM
* GNU Binutils
* GNU Make
* x86 linker scripts

The project can be built using the repository's Makefile.

```bash
make
```

The resulting artifacts are placed inside the `build/` directory.

## Project Structure

A simplified project structure looks like:

```text
NexisOS/
├── kernel/
│   ├── boot/
│   ├── drivers/
│   ├── gdt/
│   ├── handlers/
│   ├── idt/
│   ├── interrupts/
│   ├── memory/
│   ├── process/
│   └── timer/
├── build/
├── linker.ld
├── makefile
└── readme.md
```

The exact structure may evolve as the operating system grows.

## Roadmap

### Kernel

* [x] Protected mode
* [x] GDT
* [x] IDT
* [x] PIC
* [x] PIT
* [x] Basic interrupt handling
* [x] System call infrastructure
* [x] Initial process infrastructure
* [ ] Improve kernel process management
* [ ] Context switching
* [ ] Scheduler

### Memory

* [x] E820 memory map
* [x] Initial PMM
* [x] Physical page tracking
* [ ] Complete bitmap initialization
* [ ] Memory reservation
* [ ] Page allocation
* [ ] Page freeing
* [ ] Virtual memory
* [ ] Paging

### Drivers

* [x] VGA
* [x] Serial
* [x] Keyboard
* [x] PS/2 mouse
* [ ] Improved keyboard support
* [ ] Additional hardware drivers

### Shell

* [x] Input buffer
* [x] Command parsing
* [x] `help`
* [x] `clear`
* [x] `version`
* [x] Backspace
* [ ] Command arguments
* [ ] Better line editing
* [ ] More kernel commands
* [ ] Program execution

### Storage

* [ ] Disk driver
* [ ] ATA PIO
* [ ] Block device abstraction
* [ ] Filesystem
* [ ] File operations

### User Space

* [ ] User mode
* [ ] Process isolation
* [ ] Executable loading
* [ ] User-space programs
* [ ] User-space shell

## Relationship Between the Nexis Projects

NexisOS is part of a larger low-level development ecosystem.

```text
NexisLoader
     │
     ▼
  Boot Environment
     │
     ▼
   NexisK
     │
     ▼
  Kernel Services
     │
     ▼
  NexisOS
     │
     ▼
Operating System
```

Each project has a different responsibility:

| Project         | Responsibility                             |
| --------------- | ------------------------------------------ |
| **NexisLoader** | Bootloader and early system initialization |
| **NexisK**      | Kernel development                         |
| **NexisOS**     | Practical operating-system integration     |

This separation allows the kernel to evolve independently while NexisOS serves as the environment where the kernel is tested as a complete operating system.

## Limitations

NexisOS is an early-stage operating system.

It currently lacks many features expected from a general-purpose operating system, including:

* Complete virtual memory
* Complete process management
* User mode
* Filesystem
* Persistent storage support
* Full device management
* Mature scheduler
* Advanced shell functionality
* Networking
* SMP support
* Security mechanisms

These are expected limitations at this stage of development.

## Status

**Development stage:** Early OS development

**Architecture:** x86 / i386

**Kernel:** NexisK

**Bootloader:** NexisLoader

**Language:** C / NASM Assembly

**Build system:** GNU Make

**License:** GPL-2.0-only

## License

NexisOS is licensed under the **MIT**.

See the `LICENSE` file for the complete license text.

<p align="center">
  <strong>NexisOS — Putting NexisK into practice.</strong>
</p>
