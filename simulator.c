/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct
stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
static inline int32_t signExtend16(int32_t num);
static inline int32_t* checkedMemAccess(stateType* statePtr, uint32_t index);

int
main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state.numMemory) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address  %d\n. Please ensure you are providing a machine code file.", state.numMemory);
            perror("sscanf");
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    //Your code starts here!
    memset(state.reg, 0, NUMREGS * sizeof(int));
    for (size_t instructions = 0;; instructions++) {
        printState(&state);
        uint32_t instruction = state.mem[state.pc];
        state.pc++;

        uint8_t opcode = (instruction >> 22) & 0b111;
        uint8_t argA = (instruction >> 19) & 0b111;
        uint8_t argB = (instruction >> 16) & 0b111;
        int32_t argC = signExtend16(instruction & 0xffff);

        if (opcode == 0b000) {
            // ADD [reg] [reg] [dest]
            state.reg[argC] = state.reg[argA] + state.reg[argB];
        } else if (opcode == 0b001) {
            // NOR [reg] [reg] [dest]
            state.reg[argC] = ~(state.reg[argA] | state.reg[argB]);
        } else if (opcode == 0b010) {
            // LW [reg] [reg] [offset]
            state.reg[argB] = *checkedMemAccess(&state, state.reg[argA] + argC);
        } else if (opcode == 0b011) {
            // SW [reg] [reg] [offset]
            *checkedMemAccess(&state, state.reg[argA] + argC) = state.reg[argB];
        } else if (opcode == 0b100) {
            // BEQ [reg] [reg] [offset]
            if (state.reg[argA] == state.reg[argB]) {
                state.pc += argC;
            }
        } else if (opcode == 0b101) {
            // JALR [reg] [reg]
            state.reg[argB] = state.pc;
            state.pc = state.reg[argA];
        } else if (opcode == 0b110) {
            // HALT
            puts("machine halted");
            printf("total of %lu instructions executed\n", instructions + 1);
            puts("final state of machine:");
            printState(&state);
            return 0;
        }
    }

    //Your code ends here!

    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW.
*/

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++)
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++)
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int32_t signExtend16(int32_t num)
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here.
*/

static inline int32_t* checkedMemAccess(stateType* statePtr, uint32_t index) {
    if (index >= NUMMEMORY) {
        printf("Error: Illegal memory access of %d at PC=%d\n", index, statePtr->pc);
        printState(statePtr);
        exit(1);
    }

    return &(statePtr->mem[index]);
}