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

static inline int convertNum(int32_t);

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
    state.pc = 0;
    for(int i = 0; i < NUMREGS; ++i){
        state.reg[i] = 0;
    }
    if(state.numMemory==0){
        printState(&state);
        return(0);
    }
    int temp = 0;
    int count = 0;
    while(1){
        //run the code
        temp = state.mem[state.pc];
        //halt code
        printState(&state);
        ++count;
        ++state.pc;
        //DEBUGGING PURPOSES
        short debugOp = temp >> 22;
        //DEBUGGING PURPOSES

        if(debugOp==6){
            //halt instruction
            printf("machine halted\n");
            printf("total of %d instructions executed\n", count);
            printf("final state of machine:\n");
            printState(&state);
            return(0);
        }
        if(debugOp == 0){
            //add instruction
            short regA = (temp >> 19) & 7;
            short regB = (temp >> 16) & 7;
            short destReg = temp & 7;
            state.reg[destReg] = state.reg[regA] + state.reg[regB];
        }
        else if(debugOp == 1){
            //nor instruction
            short regA = (temp >> 19) & 7;
            short regB = (temp >> 16) & 7;
            short destReg = temp & 7;
            int memValue = state.reg[regA]|state.reg[regB];
            memValue = ~memValue;
            state.reg[destReg] = memValue;
        }
        else if(debugOp == 2){
            //lw instruction
            short regA = (temp>>19) & 7;
            short regB = (temp>>16) & 7;
            int offsetField = temp & ((1<<16)-1);
            offsetField = convertNum(offsetField);
            int memAddress = state.reg[regA] + offsetField;
            state.reg[regB] = state.mem[memAddress];
        }
        else if(debugOp == 3){
            //sw instruction
            short regA = (temp>>19) & 7;
            short regB = (temp>>16) & 7;
            int offsetField = temp & ((1<<16)-1);
            offsetField = convertNum(offsetField);
            int memAddress = state.reg[regA] + offsetField;
            state.mem[memAddress] = state.reg[regB];
        }
        else if(debugOp == 4){
            //beq instruction
            short regA = (temp>>19) & 7;
            short regB = (temp>>16) & 7;
            if(state.reg[regA]==state.reg[regB]){
                //
                int offsetField = temp & ((1<<16)-1);
                offsetField = convertNum(offsetField);
                state.pc = state.pc + offsetField;
            }
        }
        else if(debugOp == 5){
            //jalr instruction
            short regA = (temp>>19) & 7;
            short regB = (temp>>16) & 7;
            state.reg[regB] = state.pc;
            state.pc = state.reg[regA];
        }
        else if(debugOp == 7){
            //noop instruction
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
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/