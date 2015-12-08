#include <stdio.h>
#include <string.h>

#define N 256

typedef enum {
	JSR = 0x00,		/* jump to subroutine */
	JMP = 0x01,		/* jump */
	JPZ = 0x02,		/* jump if AC is zero */
	JNZ = 0x03,		/* jump if AC not zero */
	NND = 0x04,		/* nand */
	DNN = 0x05,		/* reverse nand */
	INC = 0x06,		/* increment */
	DEC = 0x07,		/* decrement */
	DDA = 0x08,		/* reverse add */
	BUS = 0x09,		/* reverse subtract */
	LUM = 0x0A,		/* reverse multiply */
	VID = 0x0B,		/* reverse divide */
	DOM = 0x0C,		/* reverse modulo */
	SNE = 0x0D,		/* skip if not equal */
	SGE = 0x0E,		/* skip if greater or equal */
	SLE = 0x0F,		/* skip if less or equal */
	ADD = 0x10,		/* add */
	SUB = 0x11,		/* subtract */
	MUL = 0x12,		/* multiply */
	DIV = 0x13,		/* divide */
	MOD = 0x14,		/* modulo */
	SEQ = 0x15,		/* skip if equal */
	SLT = 0x16,		/* skip if less then */
	SGT = 0x17,		/* skip if greater then */
	LAA = 0x18,		/* load address to AC */
	LAS = 0x19,		/* load address to SP */
	LDA = 0x1A,		/* load */
	STA = 0x1B,		/* store */
	ICH = 0x1C,		/* input character */
	OCH = 0x1D,		/* output character */
	INU = 0x1E,		/* input number */
	ONU = 0x1F		/* output number */
} OPCODE;

typedef enum {
	acc = 0x00,		/* accumulator */
	ind = 0x01,		/* indirect  */
	pop = 0x02,		/* postincrement(pop) */
	psh = 0x03,		/* predecrement(push)  */
	imm = 0x04,		/* immediate  */
	abs = 0x05,		/* absolute  */
	dis = 0x06,		/* displacement */
	rel = 0x07       /* relative */
} ADDRESS_MODE;

typedef struct {
	int AC;
	int SP;
	int IP;

	int memory[N];

	int program_length;

} vm_state;

typedef struct {
	OPCODE opcode;
	ADDRESS_MODE address_mode;
	
} instruction;

int mod(int w, int n)
{
	return (w % n < 0) ? (w % n + n) : (w % n);
}

void dump_memory(vm_state *vm)
{
	int i;
	printf("[");
	for(i = 0; i < N; i++)
		printf("%02X ", vm->memory[i]);
	printf("]\n");
}

void reset_machine(vm_state *vm)
{
	vm->AC = 0;
	vm->SP = 0;
	vm->IP = 0;
	vm->program_length = 0;

	memset(vm->memory, 0, sizeof(int)*N);
}

void fetch_instruction(vm_state *vm, instruction *dst)
{
	int instr = vm->memory[ vm->IP ];
	dst->opcode       = instr >> 3;
	dst->address_mode = instr & 0x7;
}


void run(vm_state *vm)
{
	instruction curr_instr;
	while(vm->IP < vm->program_length) {

		fetch_instruction(vm, &curr_instr);

	}
}

int main()
{
	int prog[] = {240, 144, 248, 236, 10};
	vm_state vm;
	reset_machine(&vm);


	memcpy(vm.memory, prog, sizeof(prog));
	vm.program_length = sizeof(prog) / sizeof(int);

	dump_memory(&vm);

	run(&vm);
	return 0;
}