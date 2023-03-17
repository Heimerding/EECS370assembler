/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);

int labelFind(char*, char[MAXLINELENGTH][MAXLINELENGTH], short);

void numcheck(char* arg0, char* arg1, char* arg2){
    if((isNumber(arg0) + isNumber(arg1) + isNumber(arg2))!=3){
        exit(1);
    }
}

void numcheck2(char* arg0, char* arg1){
    if((isNumber(arg0) + isNumber(arg1))!=2){
        exit(1);
    }
}

void bounds(int arg0, int arg1, int arg2){
    if(arg0 > 7 || arg1 > 7 || arg2 > 7){
        exit(1);
    }
    if(arg0 < 0 || arg1 < 0 || arg2 < 0){
        exit(1);
    }
}

void bounds2(int arg0, int arg1){
    bounds(arg0,arg1,0);
}

int arg2Handling(char* arg2){
    //
    int temp;
    temp = atoi(arg2);
    if(temp > 32767){
        exit(1);
    }
    if(temp < -32768){
        exit(1);
    }
    temp &= 0xFFFF;
    return temp;
}

int
main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    
    short programEnd = 0;
    short programCounter = 0;
    int argzero = 0;
    int argone = 0;
    int argtwo = 0;
    int instruction = 0;
    char labels [MAXLINELENGTH][MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    while(readAndParse(inFilePtr,label,opcode,arg0,arg1,arg2)){
        strcpy(labels[programCounter],label);
        programCounter++;
        //
    }

    programEnd = programCounter;
    programCounter = 0;
    rewind(inFilePtr);

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        //no need to handle labels
        //OPCODE handling
        if(!strcmp(opcode, "add")){
            //instruction opcode
            instruction += 0 << 22;
            numcheck(arg0,arg1,arg2);
            argzero = atoi(arg0);
            argone = atoi(arg1);
            argtwo = atoi(arg2);
            bounds(argzero,argone,argtwo);
            argzero = argzero << 19;
            argone = argone << 16;
            instruction += argzero + argone + argtwo;
        }
        else if(!strcmp(opcode, "nor")){
            //instruction opcode
            instruction += 1 << 22;
            numcheck(arg0,arg1,arg2);
            argzero = atoi(arg0);
            argone = atoi(arg1);
            argtwo = atoi(arg2);
            bounds(argzero,argone,argtwo);
            argzero = argzero << 19;
            argone = argone << 16;
            instruction += argzero + argone + argtwo;
        }
        else if(!strcmp(opcode, "lw")){
            //instruction opcode
            instruction += 2 << 22;
            numcheck2(arg0,arg1);
            argzero = atoi(arg0);
            argone = atoi(arg1);
            bounds2(argzero,argone);
            if(isNumber(arg2)){
                //
                argtwo = arg2Handling(arg2);
            }
            else{
                argtwo = labelFind(arg2, labels, programEnd);
            }
            argzero = argzero << 19;
            argone = argone << 16;
            instruction += argzero + argone + argtwo;
        }
        else if (!strcmp(opcode, "sw")){
            //instruction opcode
            instruction += 3 << 22;
            numcheck2(arg0,arg1);
            argzero = atoi(arg0);
            argone = atoi(arg1);
            bounds2(argzero,argone);
            if(isNumber(arg2)){
                argtwo = arg2Handling(arg2);
            }
            else{
                argtwo = labelFind(arg2, labels, programEnd);
            }
            argzero = argzero << 19;
            argone = argone << 16;
            instruction += argzero + argone + argtwo;
        }
        else if(!strcmp(opcode, "beq")){
            //instruction opcode
            instruction += 4 << 22;
            numcheck2(arg0,arg1);
            argzero = atoi(arg0);
            argone = atoi(arg1);
            bounds2(argzero,argone);
            if(isNumber(arg2)){
                argtwo = arg2Handling(arg2);
            }
            else{
                argtwo = labelFind(arg2, labels, programEnd); // find the address of the label
                int temp = programCounter + 1; // get the current line number
                argtwo = argtwo - temp; //find difference between address and line number - that's the difference
                if(argtwo<0){
                    argtwo = argtwo & 65535;
                }
            }
            argzero = argzero << 19;
            argone = argone << 16;
            instruction += argzero + argone + argtwo;
        }
        else if(!strcmp(opcode, "jalr")){
            //instruction opcode
            instruction += 5 << 22;
            numcheck2(arg0,arg1);
            argzero = atoi(arg0);
            argone = atoi(arg1);
            bounds2(argzero,argone);
            argzero = argzero << 19;
            argone = argone << 16;
            instruction += argzero + argone;
        }
        else if(!strcmp(opcode, "halt")){
            //instruction opcode
            instruction += 6 << 22;
        }
        else if(!strcmp(opcode, "noop")){
            //instruction opcode
            instruction += 7 << 22;
        }
        else if(!strcmp(opcode, ".fill")){
            //instruction opcode
            if(isNumber(arg0)){
                instruction = atoi(arg0);
            }
            else{
                instruction = labelFind(arg0,labels,programEnd);
            }
        }
        else{
            exit(1);
        }
        fprintf(outFilePtr,"%d\n", instruction);
        instruction = 0;
        ++programCounter;
    }
    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    // if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    //     /* reached end of file */
    // }

    // /* this is how to rewind the file ptr so that you start reading from the
    //     beginning of the file */
    // rewind(inFilePtr);

    // /* after doing a readAndParse, you may want to do the following to test the
    //     opcode */
    // if (!strcmp(opcode, "add")) {
    //     /* do whatever you need to do for opcode "add" */
    // }
    return(0);
}

int labelFind(char *label, char labels[MAXLINELENGTH][MAXLINELENGTH], short programLength){
    //
    short labelInd = 0;
    short labelRInd = programLength;

    while(strcmp(labels[labelInd], label)!= 0){
        ++labelInd;
        if(labelInd == labelRInd){
            exit(1);
        }
    }

    while(strcmp(labels[labelRInd],label)!=0){
        --labelRInd;
        if(labelRInd < labelInd){
            exit(1);
        }
    }

    if(labelInd != labelRInd){
        exit(1);
    }

    return labelInd;
}

/*
* NOTE: The code defined below is not to be modifed as it is implemented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if(nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}

