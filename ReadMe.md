# Operating System Project – egos-2000

This project explores educational enhancements made to the egos-2000 operating system. It includes kernel-level improvements, system call extensions, scheduling logic, and user-space utilities. All changes are implemented in C and demonstrate hands-on learning of core OS concepts.

---

## Overview

The egos-2000 operating system is a minimal RISC-V-based teaching OS. Modifications were made at various abstraction layers to deepen understanding of kernel internals and user-level interfaces.

---

## Key Contributions

### Process Management and System Calls

- Extended the process control block (PCB) with runtime metadata such as priority, creation time, and context switches.
- Developed system calls like `setprio` and `kill`, enabling dynamic manipulation of scheduling priority and process termination.
- Added syscall support in both kernel and libc, reflecting system-level and user-level call handling.

### Shell Development and Command Execution

- Created a custom shell (`msh`) using `fork()`, `exec*()`, and `wait()` system calls.
- Implemented built-in commands like `cd`, `exit`, and `quit`, along with support for executing external binaries.
- Added input validation and prompt behavior, handling blank lines and invalid commands gracefully.

### Scheduling Logic

- Replaced the default scheduler with a lottery-based algorithm.
- Integrated randomness using `srand()` and `rand()` to simulate probabilistic CPU allocation.
- Enabled configurable ticket counts through a new system call and observed fair distribution across processes.

### System Utility Programs

- Built user-level tools like `ps` and `kill` for process introspection and control.
- Extended `ps` to show priority and context switches, improving visibility into runtime behavior.
- Created a utility that terminates user processes (PID > 4), simplifying testing of scheduling behavior.

---

## Skills Practiced

- Low-level systems programming in C
- Kernel/user syscall interfacing
- Scheduling algorithms and process queues
- Shell construction and process control
- Debugging in minimal OS environments
- Working with the QEMU emulator and RISC-V assembly

---

## Compilation and Execution

Use the provided Makefile:

```bash
make clean
make
```

To run in QEMU:

```bash
make qemu
```

User programs like `tick`, `k`, or the shell can be invoked in the emulator as:

```bash
tick 10 5 &
k
```

---

## Author

Rency Kansagra
University of Texas at Arlington  


