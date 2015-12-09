#include <stdio.h>
#include <stdlib.h>
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
   _abs = 0x05,		/* absolute  */
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

	int operand_value;
	int operand_address;

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
	dst->opcode       = instr >>   3;
	dst->address_mode = instr  & 0x7;
}

void load_operand_acc(vm_state *vm, instruction *inst)
{
	inst->operand_address = -1;
	inst->operand_value = vm->AC;
}
void load_operand_ind(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->AC;
	inst->operand_value   = vm->memory[ inst->operand_address ];
}
void load_operand_pop(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->SP;	
	inst->operand_value   = vm->memory[ inst->operand_address ];

	vm->SP = mod(vm->SP + 1, N);
}
void load_operand_psh(vm_state *vm, instruction *inst)
{
	vm->SP = mod(vm->SP - 1, N);

	inst->operand_address = vm->SP;
	inst->operand_value   = vm->memory[ inst->operand_address ];
}
void load_operand_imm(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->IP;
	inst->operand_value   = vm->memory[ vm->IP ];

	/* Skip next instruction */
	vvm->IP = mod(vm->IP + 1, N);	
}
void load_operand_abs(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->memory[ vm->IP ];
	inst->operand_value   = vm->memory[ vm->memory[ vm->IP ] ];

	vm->IP = mod(vm->IP + 1, N);	
}
void load_operand_dis(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->memory[ vm->IP ] + vm->SP;
	inst->operand_value   = vm->memory[ vm->memory[ vm->IP ] ];
	vm->IP = mod(vm->IP + 1, N);
}
void load_operand_rel(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->memory[ vm->IP ] + vm->IP;
	inst->operand_value   = vm->memory[ vm->memory[ vm->IP ] ];
	vm->IP = mod(vm->IP + 1, N);
}

void load_operand(vm_state *vm, instruction *inst)
{
	switch (inst->address_mode)
	{
		case acc:
			load_operand_acc(vm, inst);
			break;

		case ind:
			load_operand_ind(vm, inst);
			break;

		case pop:
			load_operand_pop(vm, inst);
			break;

		case psh:
			load_operand_psh(vm, inst);
			break;

		case imm:
			load_operand_imm(vm, inst);
			break;

		case _abs:
			load_operand_abs(vm, inst);
			break;

		case dis:
			load_operand_dis(vm, inst);
			break;

		case rel:
			load_operand_rel(vm, inst);
			break;

		default:
			exit(EXIT_FAILURE);

	}
}

void op_jsr(vm_state *vm, instruction *inst)
{
	vm->AC = vm->IP;
}

void op_jsr

void execute_instruction(vm_state* vm, instruction *inst)
{
	switch (inst->opcode)
	{
		case JSR:
			op_jsr(vm, inst);
			break;

		case JMP:
			op_jmp(vm, inst);
			break;

		case JPZ:
			op_jpz(vm, inst);
			break;

		case JNZ:
			op_jnz(vm, inst);
			break;

		case NND:
			op_nnd(vm, inst);
			break;

		case DNN:
			op_ddn(vm, inst);
			break;

		case INC:
			op_inc(vm, inst);
			break;

		case DEC:
			op_dec(vm, inst);
			break;

		case DDA:
			op_dda(vm, inst);
			break;

		case BUS:
			op_bus(vm, inst);
			break;

		case LUM:
			op_lum(vm, inst);
			break;

		case VID:
			op_vid(vm, inst);
			break;

		case DOM:
			op_dom(vm, inst);
			break;

		case SNE:
			op_sne(vm, inst);
			break;

		case SGE:
			op_sge(vm, inst);
			break;

		case SLE:
			op_sle(vm, inst);
			break;

		case ADD:
			op_add(vm, inst);
			break;

		case SUB:
			op_sub(vm, inst);
			break;

		case MUL:
			op_mul(vm, inst);
			break;

		case DIV:
			op_div(vm, inst);
			break;

		case MOD:
			op_mod(vm, inst);
			break;

		case SEQ:
			op_seq(vm, inst);
			break;

		case SLT:
			op_slt(vm, inst);
			break;

		case SGT:
			op_sgt(vm, inst);
			break;

		case LAA:
			op_laa(vm, inst);
			break;

		case LAS:
			op_las(vm, inst);
			break;

		case LDA:
			op_lda(vm, inst);
			break;

		case STA:
			op_sta(vm, inst);
			break;

		case ICH:
			op_ich(vm, inst);
			break;

		case OCH:
			op_och(vm, inst);
			break;

		case INU:
			op_inu(vm, inst);
			break;

		case ONU:
			op_onu(vm, inst);
			break;

		default:
			exit(EXIT_FAILURE);
	}

}

void run(vm_state *vm)
{
	instruction curr_instr;
	while(vm->IP < vm->program_length) {

		fetch_instruction(vm, &curr_instr);
		vm->IP = mod(vm->IP + 1, N);
		load_operand(vm, &curr_instr);
		execute_instruction(vm, &curr_instr);
	}
}

int main()
{
	int prog[] = {0xF0, 0x90, 0xF8, 0xEC, 0x0A };
	vm_state vm;
	reset_machine(&vm);


	memcpy(vm.memory, prog, sizeof(prog));
	vm.program_length = sizeof(prog) / sizeof(int);

	dump_memory(&vm);

	run(&vm);
	return 0;
}