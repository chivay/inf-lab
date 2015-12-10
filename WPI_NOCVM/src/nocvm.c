#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

#define AC_ADDRESS -1

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

int nand(int a, int b)
{
	return ~(a & b);
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

/*******************************************************/
/*               OPERAND FUNCTIONS                     */

void load_operand_acc(vm_state *vm, instruction *inst)
{
	inst->operand_address = AC_ADDRESS;
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
	inst->operand_value   = vm->memory[ inst->operand_address ];

	/* Skip next instruction */
	vm->IP = mod(vm->IP + 1, N);	
}

void load_operand_abs(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->memory[ vm->IP ];
	inst->operand_value   = vm->memory[ inst->operand_address ];

	vm->IP = mod(vm->IP + 1, N);	
}

void load_operand_dis(vm_state *vm, instruction *inst)
{
	inst->operand_address = mod(vm->memory[ vm->IP ] + vm->SP, N);
	inst->operand_value   = vm->memory[ inst->operand_address ];
	vm->IP = mod(vm->IP + 1, N);
}

void load_operand_rel(vm_state *vm, instruction *inst)
{
	inst->operand_address = mod( vm->memory[ vm->IP ] + vm->IP + 1, N);
	inst->operand_value   = vm->memory[ inst->operand_address ];
	vm->IP = mod(vm->IP + 1, N);
}

void load_operand(vm_state *vm, instruction *inst)
{
	switch (inst->address_mode) {
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

/*        END OF OPERAND FUNCTIONS                     */
/*******************************************************/


/*******************************************************/
/*                OPCODE FUNCTIONS                     */

void op_jsr(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->AC = vm->IP;
	vm->IP = inst->operand_address;
}

void op_jmp(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);
	vm->IP = inst->operand_address;
}

void op_jpz(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	if (vm->AC == 0)
		vm->IP = inst->operand_address;
}

void op_jnz(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	if (vm->AC != 0)
		vm->IP = inst->operand_address;
}

void op_nnd(vm_state *vm, instruction *inst)
{
	vm->AC = nand(vm->AC, inst->operand_value);
}

void op_dnn(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		vm->AC = nand(vm->AC, inst->operand_value);
	else
		vm->memory[ inst->operand_address ] = nand(vm->AC, inst->operand_value);
}

void op_inc(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		vm->AC++;
	else
		vm->memory[ inst->operand_address ]++;
}

void op_dec(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		vm->AC--;
	else
		vm->memory[ inst->operand_address ]--;
}

void op_dda(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		vm->AC = vm->AC + inst->operand_value;
	else
		vm->memory[ inst->operand_address ] = vm->AC + inst->operand_value;
}

void op_bus(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		vm->AC = inst->operand_value -  vm->AC;
	else
		vm->memory[ inst->operand_address ] =  inst->operand_value - vm->AC;	
}

void op_lum(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		vm->AC = vm->AC * inst->operand_value;
	else
		vm->memory[ inst->operand_address ] = vm->AC * inst->operand_value;
}

void op_vid(vm_state *vm, instruction *inst)
{
	if(vm->AC == 0)
		exit(EXIT_FAILURE);

	if(inst->operand_address == AC_ADDRESS)
		vm->AC = inst->operand_value / vm->AC;
	else
		vm->memory[ inst->operand_address ] = inst->operand_value / vm->AC;
}

void op_dom(vm_state *vm, instruction *inst)
{
	if(vm->AC == 0)
		exit(EXIT_FAILURE);

	if(inst->operand_address == AC_ADDRESS)
		vm->AC = inst->operand_value % vm->AC;
	else
		vm->memory[ inst->operand_address ] = inst->operand_value % vm->AC;	
}

void op_sne(vm_state *vm, instruction *inst)
{
	if(vm->AC != inst->operand_value)
		vm->IP = mod(vm->IP + 2, N);
}

void op_sge(vm_state *vm, instruction *inst)
{
	if(vm->AC >= inst->operand_value)
		vm->IP = mod(vm->IP + 2, N);
}

void op_sle(vm_state *vm, instruction *inst)
{
	if(vm->AC <= inst->operand_value)
		vm->IP = mod(vm->IP + 2, N);
}

void op_add(vm_state *vm, instruction *inst)
{
	vm->AC += inst->operand_value;
}

void op_sub(vm_state *vm, instruction *inst)
{
	vm->AC -= inst->operand_value;
}

void op_mul(vm_state *vm, instruction *inst)
{
	vm->AC *= inst->operand_value;
}

void op_div(vm_state *vm, instruction *inst)
{
	if(inst->operand_value == 0)
		exit(EXIT_FAILURE);

	vm->AC /= inst->operand_value;
}

void op_mod(vm_state *vm, instruction *inst)
{
	if(inst->operand_value == 0)
		exit(EXIT_FAILURE);

	vm->AC %= inst->operand_value;	
}

void op_seq(vm_state *vm, instruction *inst)
{
	if(vm->AC == inst->operand_value)
		vm->IP = mod(vm->IP + 2, N);
}

void op_slt(vm_state *vm, instruction *inst)
{
	if(vm->AC < inst->operand_value)
		vm->IP = mod(vm->IP + 2, N);
}

void op_sgt(vm_state *vm, instruction *inst)
{
	if(vm->AC > inst->operand_value)
		vm->IP = mod(vm->IP + 2, N);	
}

void op_laa(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->AC = inst->operand_address;
}

void op_las(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->SP = inst->operand_address;
}

void op_lda(vm_state *vm, instruction *inst)
{
	vm->AC = inst->operand_value;
}

void op_sta(vm_state *vm, instruction *inst)
{
	if(inst->operand_address != AC_ADDRESS)
		vm->memory[ inst->operand_address ] = vm->AC;
}

void op_ich(vm_state *vm, instruction *inst)
{
	int x = getchar();

	if(x == EOF)
		x = N-1;

	if(inst->operand_address == AC_ADDRESS)
		vm->AC = x;
	else
		vm->memory[ inst->operand_address ] = x;
}

void op_och(instruction *inst)
{
	putchar(inst->operand_value);
}

void op_inu(vm_state *vm, instruction *inst)
{
	int x;
	scanf("%d", &x);

	if(x == EOF)
		x = N-1;

	if(inst->operand_address == AC_ADDRESS)
		vm->AC = x;
	else
		vm->memory[ inst->operand_address ] = x;

}

void op_onu(instruction *inst)
{
	printf("%d", inst->operand_value);
}

/*               END OF OPCODE FUNCTIONS               */
/*******************************************************/


void execute_instruction(vm_state* vm, instruction *inst)
{
	switch (inst->opcode) {
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
			op_dnn(vm, inst);
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
			op_och(inst);
			break;

		case INU:
			op_inu(vm, inst);
			break;

		case ONU:
			op_onu(inst);
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
	/* int prog[] = {0xF0, 0x90, 0xF8, 0xEC, 0x0A }; */ /* Wczytaj liczbe wypisz kwadrat */ 
	/* int prog[] = {205, 8, 210, 21, 14, 232, 13, 2, 72, 101, 108, 108, 111, 10};  */ /* Wypisz Hello*/
	/* int prog[] = {13, 4, 0, 0, 240, 108, 255, 13, 21, 221, 2, 213, 2, 21, 21, 213, 2, 69, 3, 13, 4, 253, 3, 236, 10}; */
	int prog[] = {13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 213, 12, 180, 10, 13, 28, 212, 2, 133, 12, 241, 53, 12, 13, 13, 212, 9, 221, 12, 213, 12, 108, 255, 13, 49, 212, 2, 133, 12, 249, 236, 10, 61, 12, 13, 32};
	/*int prog[] = {13, 4, 0, 0, 245, 2, 245, 3, 213, 2, 109, 3, 13, 32, 213, 2, 189, 3, 13, 26, 213, 3, 77, 2, 13, 30, 213, 2, 77, 3, 13, 8, 253, 2, 236, 10};*/

	vm_state vm;
	reset_machine(&vm);


	memcpy(vm.memory, prog, sizeof(prog));
	vm.program_length = sizeof(prog) / sizeof(int);


	run(&vm);
	return 0;
}