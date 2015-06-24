/* LC3 simulator file.
 * Complete the functions!
 * This is a front-end to the code you wrote in lc3.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc3.h"

void cmd_registers(lc3machine* mach);
void cmd_dump(lc3machine* mach, int start, int end);
void cmd_list(lc3machine* mach, int start, int end);
void cmd_setaddr(lc3machine* mach, int address, short value);
void cmd_setreg(lc3machine* mach, int reg, short value);

/* FIXME: change this to be a good prompt string */
#define PROMPT "Please type in a command: "

// Don't touch I will use this during grading.
#ifndef NO_MAIN_PLZ
int main(int argc, char **argv) 
{

	const char* prog;

	/* We expect only one argument for the program... */
	if (argc != 2) 
	{
		fprintf(stderr, "Usage: %s file.obj\n", argv[0]);
		return 1;
	}
	/* We want to open the file and make sure that it exists. */
	prog = argv[1];
	if (!fopen(prog, "r")) {
		fprintf(stderr, "Unable to open file: %s\n", prog);
		return 2;
	}

	/* Create a variable of type lc3machine here */
	/* Make a call to lc3_init to initialize your lc3machine */
	lc3machine *mach = malloc(sizeof(lc3machine));
	lc3_init(mach);
	lc3_load(mach, prog);

	/* FIXME: add your name here! */
	printf("LC-3 simulator and debugger\n");
	printf("Written by YOUR NAME HERE\n");
	printf("File given %s\n", prog);

	/* Run this loop until we are told to stop going. */
	while (1) 
	{
		printf("%s", PROMPT);
		char input[50];
		// scan the input for the command
		if (!fgets(input, sizeof(input), stdin)) {
			printf("Invalid Input.\n");
		} else {
			// get the first part (token) of the input command
			char *first = strtok(input, " \n");
			// if no command was given, continue
			if (!first) continue;
			if (strcmp(first, "quit") == 0) {
				// exit the program
				break;
			} else if (strcmp(first, "continue") == 0) {
				// run the lc3machine until it halts
				lc3_run(mach, -1);
			} else if (strcmp(first, "registers") == 0) {
				// print out registers, pc, and cc
				cmd_registers(mach);
			} else {
				// get the second part (token) of the input command
				char *second = strtok(NULL, " Rrx\n");
				if (strcmp(first, "step") == 0) {
					// run lc3machine for the given amount of step(s)
					if (second == NULL) {
						// run lc3machine for just one step
						lc3_run(mach, 1);
					} else {
						// run lc3machine for the given amount of step(s)
						lc3_run(mach, strtol(second, NULL, 10));
					}
				} else {
					// get the third part (token) of the input command
					char *third = strtok(NULL, " x\n");
					if (strcmp(first, "dump") == 0) {
						// dump (print) the value(s) of the address(es)
						if (third == NULL) {
							// dump (print) the address given
							cmd_dump(mach, strtol(second, NULL, 16), -1);
						} else {
							// dump (print) the address from the first address given to the second address given
							cmd_dump(mach, strtol(second, NULL, 16), strtol(third, NULL, 16));
						}
					} else if (strcmp(first, "list") == 0) {
						// print the disassembled instruction(s)
						if (third == NULL) {
							// print the disassembled instruction of the given address
							cmd_list(mach, strtol(second, NULL, 16), -1);
						} else {
							// print the disassembled instructions of the first address given to the second address given
							cmd_list(mach, strtol(second, NULL, 16), strtol(third, NULL, 16));
						}
					} else if (strcmp(first, "setaddr") == 0) {
						// sets the address of the given address to the given value
						cmd_setaddr(mach, strtol(second, NULL, 16), strtol(third, NULL, 10));
					} else if (strcmp(first, "setreg") == 0) {
						// sets the value of the given register to the given value
						cmd_setreg(mach, strtol(second, NULL, 10), strtol(third, NULL, 10));
					} else {
						// unknown command
						printf("Invalid Command: %s %s %s\n", first ? first : "", second ? second : "", third ? third : "");
					}
				}
			}
		}
	}

	return 0;
}
#endif // IFNDEF NO_MAIN_PLZ

/* cmd_step and cmd_continue 's functionality are provided in lc3_run
Therefore to execute the step and coninute commands you can just call lc3_run with the correct parameters*/

/* cmd_registers
Should print out all of the registers and the PC and CC in both hex and signed decimal. The format should be as follows

R0 dec|hex	R1 dec|hex	R2 dec|hex	R3 dec|hex
R4 dec|hex	R5 dec|hex	R6 dec|hex	R7 dec|hex
CC n/z/p
PC hex

hex is to be preceded with only an x and you should always print out 4 characters representing the hexadecimal representation the hex letters are CAPITALIZED.
n/z/p will only be one of the characters n, z, or p
between each register's information is a single tab.

Example output 
R0 0|x0000	R1 -1|xFFFF	R2 2|x0002	R3 32767|x7FFF
R4 31|x001F	R5 -32768|x8000	R6 6|x0006	R7 11111|x2B67
CC z
PC x3000
*/
void cmd_registers(lc3machine* mach)
{
	for (int i = 0; i < 8; i++) {
		printf("R%d %d|x%04hX%s", i, mach->regs[i], mach->regs[i], i == 3 ? "\n" : "\t");

	}
	printf("\nCC %s\n", mach->cc == LC3_NEGATIVE ? "n" : (mach->cc == LC3_ZERO ? "z" : (mach->cc == LC3_POSITIVE ? "p" : "Invalid")));
	printf("PC x%04hX\n", mach->pc);
}

/* cmd_dump
Should print out the contents of memory from start to end
If end is -1 then just print out memory[start]

Output format
addr: dec|hex

Example format
x0000: 37|x0025
x0001: 25|x0019
*/
void cmd_dump(lc3machine* mach, int start, int end)
{
	if (end == -1) end = start;
	for (unsigned short i = start; i <= end; i++) {
		printf("x%04hX:\t%d|x%04hX\n", i, mach->mem[(short) i], mach->mem[(short) i]);
	}
}

/* cmd_list
Should interpret the contents of memory from start to end as an assembled instruction
and disassemble it. e.g. if the data was x1000 then the output will be x3000
If end is -1 then just disassemble memory[start]

You will be calling lc3_disassemble to do the actual disassembling!
*/
void cmd_list(lc3machine* mach, int start, int end)
{
	if (end == -1) end = start;
	for (unsigned short i = start; i <= end; i++) {
		lc3_disassemble(mach->mem[i]);
	}
}

/* cmd_setaddr
 Should set a memory address to some value
*/
void cmd_setaddr(lc3machine* mach, int address, short value)
{
	mach->mem[address] = value;
}

/* cmd_setreg
  Should set a register to some value passed in
*/
void cmd_setreg(lc3machine* mach, int reg, short value)
{
	mach->regs[reg] = value;
}

