#include "lc3.h"

void lc3_init(lc3machine* state) 
{
	// Initialize the lc3 state according to the assignment.
   state->pc = 0x3000;
   state->cc = LC3_ZERO;
   state->halted = 0;
}

void lc3_load(lc3machine* state, const char* program)
{
	// Reread the section on the .obj file format.
	// Note you will have to load the file from disk and do File I/O.
	FILE *file = fopen(program, "rb");

	unsigned int size = 0;
	unsigned int origin = 0;
	unsigned char bytes[2];

	while (fread(bytes, 2, 1, file)) {
		// read the first 2 bytes containing the origin address value
		origin = (bytes[0] << 8) | bytes[1];
		if (fread(bytes, 2, 1, file)) {
			// read the next 2 bytes containing the size value
			size = (bytes[0] << 8) | bytes[1];
		} else {
			// return after printing the error message, when it failed to read the size value
			fprintf(stderr, "Reading Error 0\n");
			return;
		}
		for (int i = 0; i < size; i++) {
			// return after printing the error message, when it failed to read the given amount of the operation lines
			if (!fread(bytes, 2, 1, file)) {
				fprintf(stderr, "Reading Error 1\n");
				return;
			}
			state->mem[origin+i] = (bytes[0] << 8) | bytes[1];
		}
	}

	fclose(file); }

void lc3_step_one(lc3machine* state)
{
	// If the machine is not halted
	// Fetch an instruction
	// And call lc3_execute 
	if (!state->halted) {
		unsigned short instruction = lc3_fetch(state);
		lc3_execute(state, instruction);
	}
}

void lc3_run(lc3machine* state, int num_steps)
{
	if (num_steps == -1) {
		// run the lc3machine until it halts
		while (!state->halted) {
			lc3_step_one(state);
		}
	} else if (num_steps >= 0) {
		// step the given number of steps (num_steps) to run the lc3machine
		for (int i = 0; i < num_steps; i++) {
			lc3_step_one(state);
		}
	} else {
		// invalid num_steps (negative steps)
		fprintf(stderr, "Negative number of steps entered.\n");
	}
}

unsigned short lc3_fetch(lc3machine* state)
{
	/* Think back to when we first started assembly to code this */
	/* What happens during the fetch stage? */
	unsigned short instruction = state->mem[state->pc];
	state->pc++;
	return instruction;
}

void setcc(lc3machine* state, unsigned char dr)
{
	short result = state->regs[dr];
	state->cc = result > 0 ? LC3_POSITIVE : (result < 0 ? LC3_NEGATIVE : LC3_ZERO);
}

void lc3_execute(lc3machine* state, unsigned short instruction)
{
	unsigned char opcode = (unsigned char) (instruction >> 12 & 0xF);
	unsigned char dr = (unsigned char) (instruction >> 9 & 0x7);
	unsigned char sr1 = (unsigned char) (instruction >> 6 & 0x7);
	unsigned char sr2 = (unsigned char) (instruction & 0x7);
	signed char imm5 = (signed char) ((instruction & 0x1F) << 3) >> 3;
	signed char pcoffset9 = (signed char) (instruction & 0x1FF);
	unsigned char baser = (unsigned char) (instruction >> 6 & 0x7);

	if (opcode == 0x1) {
		// ADD
		if (instruction >> 5 & 1) {
			// imm5
			state->regs[dr] = state->regs[sr1] + imm5;
		} else {
			// sr2
			state->regs[dr] = state->regs[sr1] + state->regs[sr2];
		}
		setcc(state, dr);
	} else if (opcode == 0x5) {
		// AND
		if (instruction >> 5 & 1) {
			// imm5
			state->regs[dr] = state->regs[sr1] & imm5;
		} else {
			// sr2
			state->regs[dr] = state->regs[sr1] & state->regs[sr2];
		}
		setcc(state, dr);
	} else if (opcode == 0x0) {
		// BR
		unsigned char nzp = (unsigned char) (instruction >> 9 & 0x7);
		if (nzp & 4 && state->cc == LC3_NEGATIVE) {
			state->pc += pcoffset9;
		} else if (nzp & 2 && state->cc == LC3_ZERO) {
			state->pc += pcoffset9;
		} else if (nzp & 1 && state->cc == LC3_POSITIVE) {
			state->pc += pcoffset9;
		}
	} else if (opcode == 0xC) {
		// JMP or RET
		if ((instruction >> 6 & 0x7) == 7) {
			// RET
			state->pc = state->regs[7];
		} else {
			// JMP
			state->pc = state->regs[baser];
		}
	} else if (opcode == 0x4) {
		// JSR or JSRR
		state->regs[7] = state->pc;
		if (instruction >> 11 & 1) {
			// JSR
			char pcoffset11 = (char) (instruction & 0x7FF);
			state->pc += pcoffset11;
		} else {
			// JSRR
			state->pc = state->regs[baser];
		}
	} else if (opcode == 0x2) {
		// LD
		state->regs[dr] = state->mem[state->pc + pcoffset9];
		setcc(state, dr);
	} else if (opcode == 0xA) {
		// LDI
		state->regs[dr] = state->mem[state->mem[state->pc + pcoffset9]];
		setcc(state, dr);
	} else if (opcode == 0x6) {
		// LDR
		signed char offset6 = (signed char) (instruction & 0x3F);
		state->regs[dr] = state->mem[state->regs[baser] + offset6];
		setcc(state, dr);
	} else if (opcode == 0xE) {
		// LEA
		state->regs[dr] = state->pc + pcoffset9;
		setcc(state, dr);
	} else if (opcode == 0x9) {
		// NOT
		unsigned char sr = (unsigned char) (instruction >> 6 & 0x7);
		state->regs[dr] = ~state->regs[sr];
		setcc(state, dr);
	} else if (opcode == 0x8) {
		// RTI
		printf("ERROR\n");
	} else if (opcode == 0x3) {
		// ST
		unsigned char sr = (unsigned char) (instruction >> 9 & 0x7);
		state->mem[state->pc + pcoffset9] = state->regs[sr];
	} else if (opcode == 0xB) {
		// STI
		unsigned char sr = (unsigned char) (instruction >> 9 & 0x7);
		state->mem[state->mem[state->pc + pcoffset9]] = state->regs[sr];
	} else if (opcode == 0x7) {
		// STR
		unsigned char sr = (unsigned char) (instruction >> 9 & 0x7);
		signed char offset6 = (signed char) (instruction & 0x3F);
		state->mem[state->regs[baser] + offset6] = state->regs[sr];
	} else if (opcode == 0xF) {
		// TRAP
		unsigned char trapvect = (unsigned char) (instruction & 0xFF);
		lc3_trap(state, trapvect);
	} else {
		printf("ERROR\n");
	}
}

void lc3_disassemble(unsigned short instruction)
{
	// predefine the often used values
	unsigned char opcode = (unsigned char) (instruction >> 12 & 0xF);
	unsigned char dr = (unsigned char) (instruction >> 9 & 0x7);
	unsigned char sr1 = (unsigned char) (instruction >> 6 & 0x7);
	unsigned char sr2 = (unsigned char) (instruction & 0x7);
	signed char imm5 = (signed char) ((instruction & 0x1F) << 3) >> 3;
	signed char pcoffset9 = (signed char) (instruction & 0x1FF);
	unsigned char baser = (unsigned char) (instruction >> 6 & 0x7);

	if (opcode == 0x1) {
		// ADD
		if (instruction >> 5 & 1) {
			// imm5
			printf("ADD R%d, R%d, %d\n", (int) dr, (int) sr1, (int) imm5);
		} else {
			// sr2
			printf("ADD R%d, R%d, R%d\n", (int) dr, (int) sr1, (int) sr2);
		}
	} else if (opcode == 0x5) {
		// AND
		if (instruction >> 5 & 1) {
			// imm5
			printf("AND R%d, R%d, %d\n", (int) dr, (int) sr1, (int) imm5);
		} else {
			// sr2
			printf("AND R%d, R%d, R%d\n", (int) dr, (int) sr1, (int) sr2);
		}
	} else if (opcode == 0x0) {
		// BR
		unsigned char nzp = (unsigned char) (instruction >> 9 & 0x7);
		if (!nzp) {
			printf("NOP\n");
		} else {
			printf("BR%c%c%c %d\n", nzp & 4 ? 'n' : '\0', nzp & 2 ? 'z' : '\0', nzp & 1 ? 'p' : '\0', pcoffset9);
		}
	} else if (opcode == 0xC) {
		// JMP or RET
		if ((instruction >> 6 & 0x7) == 7) {
			// RET
			printf("RET\n");
		} else {
			// JMP
			printf("JMP R%d\n", (int) baser);
		}
	} else if (opcode == 0x4) {
		// JSR or JSRR
		if (instruction >> 11 & 1) {
			// JSR
			char pcoffset11 = (char) (instruction & 0x7FF);
			printf("JSR %d\n", pcoffset11);
		} else {
			// JSRR
			printf("JSRR R%d\n", (int) baser);
		}
	} else if (opcode == 0x2) {
		// LD
		printf("LD R%d, %d\n", (int) dr, (int) pcoffset9);
	} else if (opcode == 0xA) {
		// LDI
		printf("LDI R%d, %d\n", (int) dr, (int) pcoffset9);
	} else if (opcode == 0x6) {
		// LDR
		signed char offset6 = (signed char) (instruction & 0x3F);
		printf("LDR R%d, R%d, %d\n", (int) dr, (int) baser, (int) offset6);
	} else if (opcode == 0xE) {
		// LEA
		printf("LEA R%d, %d\n", (int) dr, pcoffset9);
	} else if (opcode == 0x9) {
		// NOT
		unsigned char sr = (unsigned char) (instruction >> 6 & 0x7);
		printf("NOT R%d, R%d\n", (int) dr, (int) sr);
	} else if (opcode == 0x8) {
		// RTI
		printf("ERROR\n");
	} else if (opcode == 0x3) {
		// ST
		unsigned char sr = (unsigned char) (instruction >> 9 & 0x7);
		printf("ST R%d, %d\n", (int) sr, (int) pcoffset9);
	} else if (opcode == 0xB) {
		// STI
		unsigned char sr = (unsigned char) (instruction >> 9 & 0x7);
		printf("STI R%d, %d\n", (int) sr, (int) pcoffset9);
	} else if (opcode == 0x7) {
		// STR
		unsigned char sr = (unsigned char) (instruction >> 9 & 0x7);
		signed char offset6 = (signed char) (instruction & 0x3F);
		printf("STR R%d, R%d, %d\n", (int) sr, (int) baser, (int) offset6);
	} else if (opcode == 0xF) {
		// TRAP
		unsigned char trapvect = (unsigned char) (instruction & 0xFF);
		printf("TRAP x%X\n", (int) trapvect);
	} else {
		printf("ERROR\n");
	}
}

void lc3_trap(lc3machine* state, unsigned char vector8)
{
	// set the pc to be the return address (regs[7])
	state->regs[7] = state->pc;
	if (vector8 == 0x20) {
		// GETC: scan a character and save the value of the scanned character into R0
		char input = getchar();
		state->regs[0] = input;
	} else if (vector8 == 0x21) {
		// OUT: print the current value of R0 as a character
		putchar(state->regs[0] & 0xFF);
	} else if (vector8 == 0x22) {
		// PUTS: print the characters in starting from the address in R0 until the end
		unsigned int index = state->regs[0];
		while (state->mem[index] != 0) {
			putchar(state->mem[index]);
			index++;
		}
	} else if (vector8 == 0x23) {
		// IN: print the prompt and scan a character and save the value of the scanned character into R0
		printf("Input character: ");
		char input = getchar();
		state->regs[0] = input;
	} else if (vector8 == 0x24) {
		// PUTSP: print the characters starting from the address in R0 until the end, two characters per memory location
		unsigned int index = state->regs[0];
		while (state->mem[index] != 0) {
			char c0 = (char) (state->mem[index] & 0xFF);
			char c1 = (char) ((state->mem[index] >> 8) & 0xFF);
			if (c1 != 0) printf("%c%c", c0, c1);
			else printf("%c", c0);
			index++;
		}
	} else if (vector8 == 0x25) {
		// HALT
		state->halted = 1;
	} else {
		// set the current pc to the address in the memory of the given address (vector8)
		state->pc = state->mem[vector8];
	}
}
