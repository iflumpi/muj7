# muj7
This project is a toolset for muj7, a fictitious CPU architecture with a limited set of instructions. The toolset includes an assembler, a debugger, a CPU emulator, and a simple machine emulator with video memory.
These tools are written in C. The project has been developed as part of a challenge, and can be used for educational purposes to teach low-level computing concepts.

## Architecture
The muj7 architecture is based in a 8-bit CPU with 3 general-purpose registers and 19 instructions. This CPU is simple yet powerful enough to perform any computational task.

### Registers
- The CPU contains 3 general-purpose 8-bit registers, labeled **A**, **B**, and **C**. These registers can be used to hold data, addresses, or intermediate results.
- The flags register (**F**) holds flags that provides information of the result of an operation (e.g., zero flag, carry flag).  
- The program counter register (**PC**) is a special 16-bit register that holds the address of next instruction to be executed.

### Instruction set
The CPU supports a small but functional set of 19 instructions. These instructions are encoded as 8-bit binary values. 

- 0x00: **NOP**
- 0x01: **LD B,A**
- 0x02  **LD C,A**
- 0x03: **LD B,C**
- 0x04: **LD A, value**
- 0x05: **ADD B**
- 0x06: **ADD C**
- 0x07: **SUB B**
- 0x08: **SUB C**
- 0x09: **STORE A,[AB]**
- 0x0A: **STORE A,[BC]**
- 0x0B: **INC A**
- 0x0C: **INC B**
- 0x0D: **INC C**
- 0x0E: **DEC A**
- 0x0F: **DEC B**
- 0x10: **DEC C**
- 0x0B: **JP address**
- 0x0C: **JPZ address**


## muj7 tools

### CPU Emulator
The CPU emulator simulates the behavior of the muj7 CPU, allowing you to run muj7 machine code on your local machine without the need for physical hardware. It accurately replicates the execution of instructions and the state of the CPU, making it a useful tool for testing and development.

### Assembler
The assembler converts assembly code written for the muj7 architecture into machine code that can be executed by the muj7 CPU. It supports all the instructions of the muj7 architecture and can handle labels and variables.

### Debugger
The debugger is a VI-like TUI that allows to step through the execution of muj7 machine code, inspect memory and registers, and analyze the state of the CPU during execution. It provides a valuable tool for debugging and optimizing code for the muj7 architecture.

### Machine Emulator
The machine emulator provides a simulated environment for running programs in an emulator of a machine with muj7 CPU and video memory for displaying output. It allows you to test your code in a realistic environment and see the results of your programs visually.

## Usage

1. Clone the repository:

    ```bash
    git clone https://github.com/iflumpi/muj7.git
    cd muj7
    ```

2. Build project:

    ```bash
    make
    ```

    or build each tool one by one

    ```bash
    make cpu
    make assembler
    make debugger
    make machine
    ```

3. Use of assembler:

    ```bash
    ./muj7.sh -a examples/program_2.asm -o examples/program_2.bin
    ```

4. Use of debugger:

    ```bash
    ./muj7.sh -d examples/program_2.bin
    ```

5. Use of machine:

    ```bash
    ./muj7.sh -m examples/program_2.bin
    ```

    (The program_2 code is writing directly some bytes to the video memory)
