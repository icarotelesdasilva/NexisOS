# NexisOS

<p align="center">
  <strong>Practical Operating System Built Around the NexisK Kernel</strong>
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#goals">Goals</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#nexisk">NexisK</a> •
  <a href="#nexisloader">NexisLoader</a> •
  <a href="#memory-management">Memory Management</a> •
  <a href="#interrupts">Interrupts</a> •
  <a href="#current-status">Current Status</a> •
  <a href="#building">Building</a> •
  <a href="#roadmap">Roadmap</a>
</p>

## Overview

**NexisOS** is the practical operating system built around the **NexisK kernel**.

The project exists to take the kernel architecture and subsystems developed in NexisK and integrate them into a complete, bootable operating-system environment.

Instead of treating kernel components as isolated experiments, NexisOS brings together the bootloader, kernel, memory management, interrupt handling, hardware drivers, input system, terminal, and shell into a single system that can be booted and tested as an operating system.

> **NexisK develops the kernel. NexisOS puts the kernel into practice.**

NexisOS is developed from low-level fundamentals, with direct interaction with x86 hardware and minimal dependence on external operating-system infrastructure.

The project currently targets **32-bit x86 (i386)** hardware and is designed around the Nexis ecosystem's own boot and kernel components.

## Goals

The primary goal of NexisOS is to provide a real environment in which NexisK can be integrated, executed, tested, and evolved.

The project focuses on:

* Integrating NexisK into a bootable operating system.
* Using NexisLoader as the system boot environment.
* Managing physical memory.
* Processing the system's E820 memory map.
* Handling hardware interrupts.
* Communicating directly with hardware devices.
* Providing keyboard input.
* Providing a VGA-based terminal.
* Providing a basic interactive shell.
* Experimenting with processes and scheduling.
* Testing the system under x86 emulation and real hardware.
* Keeping the implementation close to the underlying hardware.

NexisOS is not intended to reproduce an existing operating system.

It is an independent systems-programming project built from low-level components.

## Architecture

NexisOS is composed of several independent projects and subsystems that operate together as a single bootable system.

```text
                         NexisOS
                    Operating System
                           │
                           ▼
                        NexisK
                         Kernel
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
       Memory          Interrupts        Drivers
       Manager            │                │
          │            IDT / PIC       VGA / Serial
         PMM               │            Keyboard
          │              PIT              Mouse
          └────────────────┼────────────────┘
                           │
                           ▼
                     NexisLoader
                       Bootloader
                           │
                           ▼
                      x86 Hardware
```

The boot chain is conceptually:

```text
BIOS
 │
 ▼
NexisLoader
 │
 ├── Detect system memory
 ├── Prepare boot environment
 ├── Load NexisK
 │
 ▼
NexisK
 │
 ├── Initialize CPU environment
 ├── Initialize interrupts
 ├── Initialize memory
 ├── Initialize hardware
 └── Start kernel services
 │
 ▼
NexisOS
```

The separation between projects allows each component to have a clearly defined responsibility.

## NexisK

**NexisK** is the kernel at the center of NexisOS.

It provides the low-level execution environment and contains the fundamental operating-system mechanisms used by NexisOS.

NexisK currently targets **32-bit x86 (i386)** and is developed primarily using:

* C
* NASM Assembly
* Freestanding development
* Direct hardware access

Current kernel subsystems include:

* Global Descriptor Table (GDT)
* Interrupt Descriptor Table (IDT)
* Programmable Interrupt Controller (PIC)
* Programmable Interval Timer (PIT)
* Physical Memory Manager (PMM)
* VGA text output
* Serial output
* PS/2 keyboard handling
* PS/2 mouse handling
* System call infrastructure
* Initial process infrastructure

NexisOS acts as the practical environment in which these kernel components are integrated and tested together.

## NexisLoader

**NexisLoader** is the custom BIOS bootloader used by the Nexis ecosystem.

It is maintained as an independent project from NexisK and NexisOS.

Its responsibilities include:

* Initial BIOS bootstrapping
* Loading the next boot stage
* Loading the kernel
* Kernel selection
* E820 memory map detection
* Passing system information to the kernel
* Preparing the environment required by NexisK

The separation between NexisLoader and NexisK is intentional.

The bootloader is responsible for establishing the environment required to start the kernel, while NexisK is responsible for the operating-system kernel itself.

NexisOS then provides the practical operating-system environment in which the kernel operates.

## Memory Management

NexisOS uses the memory information provided by NexisLoader to initialize physical memory management inside NexisK.

The current implementation uses the **BIOS E820 memory map** to identify usable physical memory regions.

Usable regions are divided into **4 KiB pages**.

The current physical memory representation uses a bitmap-like byte array in which each physical page is represented by one byte.

```text
E820 Memory Map
       │
       ▼
Identify usable regions
       │
       ▼
Calculate page count
       │
       ▼
Calculate physical addresses
       │
       ▼
Calculate page indices
       │
       ▼
Track usable physical pages
```

The current PMM implementation is an initial stage.

The planned memory-management architecture includes:

* Physical page allocation
* Physical page freeing
* Memory reservation
* Complete bitmap initialization
* Paging
* Virtual memory
* Kernel memory protection

## Interrupts

Interrupt handling is a fundamental part of NexisOS.

The kernel currently uses:

* IDT
* PIC
* IRQ handlers
* Assembly interrupt entry points
* C interrupt handlers

Hardware events enter the kernel through the interrupt subsystem.

For example, the PS/2 keyboard generates **IRQ1**.

The general interrupt path is:

```text
Hardware Device
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
Kernel Subsystem
```

This structure keeps hardware interrupt entry controlled and separated from higher-level kernel logic.

## Keyboard and Shell

NexisOS currently provides keyboard input through the PS/2 keyboard interface.

Keyboard scancodes are processed by the kernel and translated into characters for the shell.

The system provides a basic command-line interface:

```text
NexisK> help

Available commands:
  help
  clear
  version
```

Current shell functionality includes:

* Character input
* Numeric input
* Spaces
* Enter
* Backspace
* Input buffering
* Basic command parsing
* Basic commands

The shell is intentionally small.

Its primary purpose is to provide a practical interface for interacting with and testing the kernel while the operating system continues to evolve.

## VGA Terminal

NexisOS currently uses VGA text mode for terminal output.

The VGA subsystem provides:

* Direct VGA memory access
* Text output
* Screen clearing
* Cursor position tracking
* Newline handling
* Backspace handling

The cursor position is maintained by the VGA driver itself.

Higher-level components such as the shell do not need to directly manipulate VGA memory.

This keeps terminal behavior separated from shell logic.

## Current Status

NexisOS is currently in **early operating-system development**.

The project has progressed beyond isolated kernel experiments and is focused on integrating the bootloader, kernel, memory management, interrupt system, drivers, input handling, and shell into a single bootable system.

### Implemented

* [x] x86 kernel boot
* [x] 32-bit protected mode
* [x] GDT
* [x] IDT
* [x] PIC
* [x] PIT
* [x] Serial output
* [x] VGA text output
* [x] Keyboard IRQ
* [x] Keyboard input
* [x] Basic shell
* [x] Shell input buffer
* [x] Basic command parsing
* [x] `help`
* [x] `clear`
* [x] `version`
* [x] Backspace handling
* [x] E820 memory map integration
* [x] Initial physical memory manager
* [x] Physical page tracking
* [x] PS/2 mouse infrastructure
* [x] Initial process infrastructure

### In Development

* [ ] Complete physical page allocation
* [ ] Physical page freeing
* [ ] Complete memory reservation
* [ ] Paging
* [ ] Virtual memory
* [ ] Process execution
* [ ] Context switching
* [ ] Scheduler
* [ ] Storage subsystem
* [ ] Filesystem
* [ ] User mode
* [ ] User-space programs

## Features

### Kernel

* 32-bit x86 kernel
* Freestanding C environment
* NASM Assembly
* Protected mode execution
* GDT
* IDT
* PIC
* PIT
* Hardware interrupt handling
* System call infrastructure
* Initial process infrastructure

### Memory

* E820 memory map
* Initial physical memory manager
* 4 KiB page tracking
* Bitmap-based physical page representation
* Usable memory detection

### Hardware

* VGA text mode
* Serial output
* PS/2 keyboard
* PS/2 mouse
* Programmable Interrupt Controller
* Programmable Interval Timer

### User Interface

* VGA terminal
* Keyboard input
* Interactive shell
* Input buffering
* Command parsing
* `help`
* `clear`
* `version`
* Backspace support

### Boot

* Custom NexisLoader boot chain
* BIOS boot support
* Kernel loading
* E820 memory detection
* Bootable disk image
* Bootable ISO generation

## Development Philosophy

NexisOS follows one fundamental principle:

> **Build the system from the lowest practical level upward.**

The project avoids hiding the operating system behind large external frameworks.

Subsystems are implemented directly, tested independently, and then integrated into the complete system.

This makes failures useful.

When something breaks, the objective is to identify the layer responsible for the failure rather than hide it behind a high-level abstraction.

NexisOS also acts as a practical validation environment for NexisK.

A subsystem that works in isolation is useful.

A subsystem that continues working after being integrated with the bootloader, kernel, interrupt subsystem, memory manager, drivers, and shell is a much stronger validation of the implementation.

## Testing

NexisOS is intended to be tested in both virtualized environments and physical x86 hardware.

### Emulation and Virtual Machines

Virtual machines and x86 emulators are useful during development because they provide:

* Fast iteration
* Reproducible environments
* Easier debugging
* Kernel crash investigation
* Hardware-independent development

The primary development workflow can use QEMU.

### Real Hardware

Real hardware is also an important validation target.

Physical machines can expose behavior that may not appear under emulation, including:

* Firmware differences
* Hardware initialization differences
* Timing differences
* Different memory layouts
* Device-specific behavior

NexisOS therefore aims to validate the system on actual x86 hardware as development progresses.

## Building

NexisOS uses a freestanding x86 build environment.

Required development tools include:

* GCC
* NASM
* GNU Binutils
* GNU Make
* An i386-compatible linker

The repository's Makefile builds the boot components, kernel objects, final boot image, and bootable ISO.

Build the system with:

```bash
make
```

The final ISO is generated inside the `build/` directory.

The generated image can then be used with QEMU or written to appropriate boot media for hardware testing.

### Run with QEMU

```bash
make run
```

### Development / Debug Mode

```bash
make dev
```

### Clean Build Artifacts

```bash
make clean
```

The build process follows this general pipeline:

```text
NexisK Sources
      │
      ▼
C / Assembly Objects
      │
      ▼
   Linker
      │
      ▼
NexisK Binary
      │
      ▼
NexisLoader + NexisK
      │
      ▼
Boot Image
      │
      ▼
Bootable ISO
```

The ISO is the final distributable boot image produced by the build system.

## Project Structure

A simplified project structure is:

```text
NexisOS/
├── boot/
│   └── stage2.S
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
├── Makefile
├── LICENSE
└── README.md
```

The exact directory structure may evolve as the operating system grows.

Generated build artifacts are kept inside `build/`.

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
* [ ] Context switching
* [ ] Process management
* [ ] Scheduler
* [ ] Improved kernel services

### Memory

* [x] E820 memory map
* [x] Initial PMM
* [x] Physical page tracking
* [ ] Complete bitmap initialization
* [ ] Memory reservation
* [ ] Page allocation
* [ ] Page freeing
* [ ] Paging
* [ ] Virtual memory

### Drivers

* [x] VGA
* [x] Serial
* [x] Keyboard
* [x] PS/2 mouse
* [ ] Improved keyboard support
* [ ] Additional hardware drivers
* [ ] Storage drivers

### Shell

* [x] Input buffer
* [x] Character input
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
* [ ] Persistent storage

### User Space

* [ ] User mode
* [ ] Process isolation
* [ ] Executable loading
* [ ] User-space programs
* [ ] User-space shell

## Relationship Between the Nexis Projects

NexisOS is part of a broader low-level systems ecosystem.

The projects have distinct responsibilities:

| Project         | Responsibility                         |
| --------------- | -------------------------------------- |
| **NexisLoader** | Bootloader and early boot environment  |
| **NexisK**      | Kernel development                     |
| **NexisOS**     | Practical operating-system integration |

The relationship can be represented as:

```text
NexisLoader
     │
     │ boots and prepares
     ▼
   NexisK
     │
     │ provides kernel services
     ▼
  NexisOS
     │
     │ integrates and exercises
     ▼
Operating System
```

This separation allows NexisLoader to evolve as a boot project, NexisK to remain focused on kernel development, and NexisOS to serve as the environment where the kernel is used as a complete operating system.

## Limitations

NexisOS is an early-stage operating system and is not intended to provide the feature set of a general-purpose operating system.

Current limitations include:

* 32-bit x86 architecture
* No complete virtual memory subsystem
* No mature process scheduler
* No user mode
* No filesystem
* No persistent storage layer
* Limited device support
* Limited shell functionality
* No networking stack
* No SMP support
* Limited security mechanisms

These limitations are expected at the current development stage.

## Status

**Project:** NexisOS

**Release:** OS v0.0.8

**Development Stage:** Early OS development

**Architecture:** x86 / i386

**Kernel:** NexisK

**Bootloader:** NexisLoader

**Languages:** C / NASM Assembly

**Build System:** GNU Make

**Boot Format:** Bootable ISO

**License:** GPL-2.0-only

## License

NexisOS is licensed under the **GNU General Public License v2.0 only (GPL-2.0-only)**.

See the `LICENSE` file for the complete license text.

<p align="center">
  <strong>NexisOS — Putting NexisK into practice.</strong>
</p>
