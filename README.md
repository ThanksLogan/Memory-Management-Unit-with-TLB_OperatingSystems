# Memory Management Unit with TLB

## Overview
This C++ project simulates a Memory Management Unit (MMU) with a Translation Lookaside Buffer (TLB). It processes virtual memory addresses from a trace file, translating them into physical addresses using a page table and TLB cache mechanism. The project demonstrates concepts like virtual-to-physical address translation, TLB caching, page table lookups, and LRU (Least Recently Used) cache replacement policy.

## Getting Started

### Prerequisites
- C++ compiler (e.g., g++)
- Standard C++ libraries
- Make utility for build automation (optional)

### Installation and Compilation
1. Ensure that a C++ compiler is installed on your machine.
2. Clone or download the project repository.
3. Navigate to the project directory and use the Makefile to compile:
```
make
```

This will create the executable mmuwithtlb.

## Running the Program

Execute the program with the following command, replacing [trace_file] with the path to your trace file, and [bit_counts] with the bit counts for each level in the page table:

```
./mmuwithtlb [trace_file] [bit_counts...]
```

## Code Description

### Files and Modules
- `TLB.h` & `TLB.cpp`: Implements the Translation Lookaside Buffer (TLB).
- `pagetable.h` & `pagetable.cpp`: Manages the page table for address translation.
- `vaddr_tracereader.h` & `vaddr_tracereader.c`: Reads and processes virtual address traces.
- `print_helpers.h` & `print_helpers.c`: Utilities for printing various outputs.
- `main.cpp`: Main driver of the program.
- `Makefile`: Script for compiling the program.

### Key Components
- **TLB (Translation Lookaside Buffer):**
  - A cache mechanism for speeding up virtual-to-physical address translation.
  - Implements LRU (Least Recently Used) policy for cache replacement.

- **Page Table:**
  - Handles the mapping of virtual addresses to physical frames.
  - Supports multi-level page table structure.

- **Address Translation:**
  - Processes addresses from a trace file.
  - Translates virtual addresses to physical addresses using the page table and TLB.


### Compilation Flags
- `-std=c++11`: Uses the C++11 standard.
- `-Wall`: Enables all compiler's warning messages.
- `-g3`: Includes extra debugging information.

### Makefile Targets
- `$(PROGRAM)`: Compiles the main program.
- `clean`: Cleans up compiled objects and executable.

### Testing
- The program can be tested with a trace file containing virtual addresses.
- It will output the physical addresses, TLB and page table hits/misses, and other relevant information.

```
./mmuwithtlb trace3.tr -p va2pa_tlb_ptwalk 20 2 -c 15
```

![image](https://github.com/ThanksLogan/CS480-A3/assets/89110766/0e64246a-901a-4390-b29c-1ab13abfbba4)

