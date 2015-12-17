#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/* Size of memory and word in memory */
#define N 256

/* Max code size */
#define CODE_SIZE 10000

/* Virtual address of AC register */
#define AC_ADDRESS -1

#define COMMENT_SYMBOL ';'

/* Operations codes available to use */
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

/* Types of addressing modes of operands */
typedef enum {
	acc = 0x00,		/* accumulator */
	ind = 0x01,		/* indirect  */
	pop = 0x02,		/* postincrement(pop) */
	psh = 0x03,		/* predecrement(push)  */
	imm = 0x04,		/* immediate  */
   _abs = 0x05,		/* absolute  */
	dis = 0x06,		/* displacement */
	rel = 0x07      /* relative */
} ADDRESS_MODE;


/* Structure containing state of VM */
typedef struct {
	int AC;
	int SP;
	int IP;

	int memory[N];

	int program_length;
} vm_state;


/* Structure containing parameters of current instruction */
typedef struct {
	OPCODE opcode;
	ADDRESS_MODE address_mode;

	int operand_value;
	int operand_address;
} instruction;

/* Stack implementation */
typedef struct {
	/* Array containing stack, maximum size = N */
	int s[N];
	/* Index of first element */
	int top;
} stack;

void push_stack(stack *st, int value)
{
	/* There is still some space*/
	if (st->top < N-1)
	{
		st->top++;
		st->s[st->top] = value;
	}
}

int pop_stack(stack *st)
{
	if (st->top != -1)
		return st->s[st->top--];
	else
		return -1;
}

void init_stack(stack *st)
{
	st->top = -1;
}

/* End of stack implementation */

/* Computes computes math-like remainder of @w / @n, not the same as % */
int modulo(int w, int n)
{
	return (w % n < 0) ? (w % n + n) : (w % n);
}

/* Computes modulo of @w by N */
int mod(int w)
{
	return modulo(w, N);
}

/* Computes bitwise NAND of @a and @b */
int nand(int a, int b)
{
	return ~(a & b);
}

bool scan_int(char *code, int *value, int *bytes)
{
	return sscanf(code, "%d%n", value, bytes);
}

bool isletter(char c)
{
	return (c >= 'a' && c <= 'z');
}

char get_escape_sequence(char c)
{
	switch (c){
		case 'a':  return '\a';
		case 'b':  return '\b';
		case 'f':  return '\f';
		case 'n':  return '\n';
		case 'r':  return '\r';
		case 't':  return '\t';
		case 'v':  return '\v';
		case '\\': return '\\';
		case '"':  return '\"';
		default:   return '\0';			
	}
}

/* Gets next instruction to process from address pointed by IP */
void fetch_instruction(vm_state *vm, instruction *dst)
{
	int instr = vm->memory[ vm->IP ];

	dst->opcode       = instr >>   3; /* inst / 8 */
	dst->address_mode = instr  & 0x7; /* inst % 8 */
}

/*******************************************************/
/*               OPERAND FUNCTIONS                     */
/*******************************************************/

/* Handles addressing mode: acc
 * operand: ACC register
 * stores operand address and value in @inst
 */
void load_operand_acc(vm_state *vm, instruction *inst)
{
	inst->operand_address = AC_ADDRESS;
	inst->operand_value = vm->AC;
}

/* Handles addressing mode: ind
 * operand: word pointed by AC
 * stores operand address and value in @inst
 */
void load_operand_ind(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->AC;
	inst->operand_value   = vm->memory[ inst->operand_address ];
}

/* Handles addressing mode: pop
 * operand: word pointed by SP 
 * stores operand address and value in @inst, moves SP to next word
 */
void load_operand_pop(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->SP;	
	inst->operand_value   = vm->memory[ inst->operand_address ];

	vm->SP = mod(vm->SP + 1);
}

/* Handles addressing mode: psh
 * operand: word pointed by SP 
 * moves SP to previous word, stores operand address and value in @inst
 */
void load_operand_psh(vm_state *vm, instruction *inst)
{
	vm->SP = mod(vm->SP - 1);

	inst->operand_address = vm->SP;
	inst->operand_value   = vm->memory[ inst->operand_address ];
}

/* Handles addressing mode: imm
 * operand: word pointed by IP 
 * stores operand address and value in @inst
 */
void load_operand_imm(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->IP;
	inst->operand_value   = vm->memory[ inst->operand_address ];

	/* Skip next instruction */
	vm->IP = mod(vm->IP + 1);	
}

/* Handles addressing mode: abs
 * operand: word at address pointed by word pointed by IP
 * stores operand address and value in @inst
 */
void load_operand_abs(vm_state *vm, instruction *inst)
{
	inst->operand_address = vm->memory[ vm->IP ];
	inst->operand_value   = vm->memory[ inst->operand_address ];

	/* Skip next instruction */
	vm->IP = mod(vm->IP + 1);	
}

/* Handles addressing mode: dis
 * operand: word at address = word poined by IP + SP
 * stores operand address and value in @inst
 */
void load_operand_dis(vm_state *vm, instruction *inst)
{
	inst->operand_address = mod(vm->memory[ vm->IP ] + vm->SP);
	inst->operand_value   = vm->memory[ inst->operand_address ];

	/* Skip next instruction */
	vm->IP = mod(vm->IP + 1);
}

/* Handles addressing mode: rel
 * operand: word at address = word poined by IP + SP + 1
 * stores operand address and value in @inst
 */
void load_operand_rel(vm_state *vm, instruction *inst)
{
	inst->operand_address = mod(vm->memory[ vm->IP ] + vm->IP + 1);
	inst->operand_value   = vm->memory[ inst->operand_address ];

	/* Skip next instruction */
	vm->IP = mod(vm->IP + 1);
}

/* Runs function responsible for loading operand for current instrution */
void load_operand(vm_state *vm, instruction *inst)
{
	switch (inst->address_mode) {
		case acc: load_operand_acc(vm, inst); break;
		case ind: load_operand_ind(vm, inst); break;
		case pop: load_operand_pop(vm, inst); break;
		case psh: load_operand_psh(vm, inst); break;
		case imm: load_operand_imm(vm, inst); break;
	   case _abs: load_operand_abs(vm, inst); break;
		case dis: load_operand_dis(vm, inst); break;
		case rel: load_operand_rel(vm, inst); break;
		default:
			exit(EXIT_FAILURE);
	}
}

/*******************************************************/
/*            END OF OPERAND FUNCTIONS                 */
/*******************************************************/


/*******************************************************/
/*                OPCODE FUNCTIONS                     */
/*******************************************************/

/* Saves mod( @value ) in address @op_adress,  */
void save_to_operand(vm_state *vm, int op_address, int value)
{
	if (op_address == AC_ADDRESS)
		vm->AC = mod(value);
	else
		vm->memory[op_address] = mod(value);
}

/* Handles opcode: JSR */
void op_jsr(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->AC = vm->IP;
	vm->IP = inst->operand_address;
}

/* Handles opcode: JMP */
void op_jmp(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->IP = inst->operand_address;
}

/* Handles opcode: JPZ */
void op_jpz(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	if (vm->AC == 0)
		vm->IP = inst->operand_address;
}

/* Handles opcode: JNZ */
void op_jnz(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	if (vm->AC != 0)
		vm->IP = inst->operand_address;
}

/* Handles opcode: NND */
void op_nnd(vm_state *vm, instruction *inst)
{
	vm->AC = nand(vm->AC, inst->operand_value);
}

/* Handles opcode: DNN */
void op_dnn(vm_state *vm, instruction *inst)
{
	int res = nand(vm->AC, inst->operand_value);
	save_to_operand(vm, inst->operand_address, res);
}

/* Handles opcode: INC */
void op_inc(vm_state *vm, instruction *inst)
{
	int res = inst->operand_value + 1;
	save_to_operand(vm, inst->operand_address, res);
}

/* Handles opcode: DEC */
void op_dec(vm_state *vm, instruction *inst)
{
	int res = inst->operand_value - 1;
	save_to_operand(vm, inst->operand_address, res);
}

/* Handles opcode: DDA */
void op_dda(vm_state *vm, instruction *inst)
{
	int res = vm->AC + inst->operand_value;
	save_to_operand(vm, inst->operand_address, res);
}

/* Handles opcode: BUS */
void op_bus(vm_state *vm, instruction *inst)
{
	int res = inst->operand_value - vm->AC;
	save_to_operand(vm, inst->operand_address, res);	
}

/* Handles opcode: LUM */
void op_lum(vm_state *vm, instruction *inst)
{
	int res = inst->operand_value * vm->AC;
	save_to_operand(vm, inst->operand_address, res);
}

/* Handles opcode: VID */
void op_vid(vm_state *vm, instruction *inst)
{
	int res;
	if(vm->AC == 0)
		exit(EXIT_FAILURE);

	res = inst->operand_value / vm->AC;
	save_to_operand(vm, inst->operand_address, res);
}

/* Handles opcode: DOM */
void op_dom(vm_state *vm, instruction *inst)
{
	int res;
	if(vm->AC == 0)
		exit(EXIT_FAILURE);

	res = inst->operand_value % vm->AC;
	save_to_operand(vm, inst->operand_address, res);	
}

/* Handles opcode: SNE */
void op_sne(vm_state *vm, instruction *inst)
{
	if(vm->AC != inst->operand_value)
		vm->IP = mod(vm->IP + 2);
}

/* Handles opcode: SGE */
void op_sge(vm_state *vm, instruction *inst)
{
	if(vm->AC >= inst->operand_value)
		vm->IP = mod(vm->IP + 2);
}

/* Handles opcode: SLE */
void op_sle(vm_state *vm, instruction *inst)
{
	if(vm->AC <= inst->operand_value)
		vm->IP = mod(vm->IP + 2);
}

/* Handles opcode: ADD */
void op_add(vm_state *vm, instruction *inst)
{
	int res = vm->AC + inst->operand_value;
	save_to_operand(vm, AC_ADDRESS, res);
}

/* Handles opcode: SUB */
void op_sub(vm_state *vm, instruction *inst)
{
	int res = vm->AC - inst->operand_value;
	save_to_operand(vm, AC_ADDRESS, res);
}

/* Handles opcode: MUL */
void op_mul(vm_state *vm, instruction *inst)
{
	int res = vm->AC * inst->operand_value;
	save_to_operand(vm, AC_ADDRESS, res);
}

/* Handles opcode: DIV */
void op_div(vm_state *vm, instruction *inst)
{
	int res;
	if(inst->operand_value == 0)
		exit(EXIT_FAILURE);

	res = vm->AC / inst->operand_value;
	save_to_operand(vm, AC_ADDRESS, res);
}

/* Handles opcode: MOD */
void op_mod(vm_state *vm, instruction *inst)
{
	int res;
	if(inst->operand_value == 0)
		exit(EXIT_FAILURE);

	res = vm->AC % inst->operand_value;
	save_to_operand(vm, AC_ADDRESS, res);
}

/* Handles opcode: SEQ */
void op_seq(vm_state *vm, instruction *inst)
{
	if(vm->AC == inst->operand_value)
		vm->IP = mod(vm->IP + 2);
}

/* Handles opcode: SLT */
void op_slt(vm_state *vm, instruction *inst)
{
	if(vm->AC < inst->operand_value)
		vm->IP = mod(vm->IP + 2);
}

/* Handles opcode: SGT */
void op_sgt(vm_state *vm, instruction *inst)
{
	if(vm->AC > inst->operand_value)
		vm->IP = mod(vm->IP + 2);	
}

/* Handles opcode: LAA */
void op_laa(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->AC = inst->operand_address;
}

/* Handles opcode: LAS */
void op_las(vm_state *vm, instruction *inst)
{
	if(inst->operand_address == AC_ADDRESS)
		exit(EXIT_SUCCESS);

	vm->SP = inst->operand_address;
}

/* Handles opcode: LDA */
void op_lda(vm_state *vm, instruction *inst)
{
	vm->AC = inst->operand_value;
}

/* Handles opcode: STA */
void op_sta(vm_state *vm, instruction *inst)
{
	if(inst->operand_address != AC_ADDRESS)
		vm->memory[ inst->operand_address ] = vm->AC;
}

/* Handles opcode: ICH */
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

/* Handles opcode: OCH */
void op_och(instruction *inst)
{
	putchar(inst->operand_value);
}

/* Handles opcode: INU */
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

/* Handles opcode: ONU */
void op_onu(instruction *inst)
{
	printf("%d", inst->operand_value);
}

/*******************************************************/
/*               END OF OPCODE FUNCTIONS               */
/*******************************************************/

/*******************************************************/
/*                 EXECUTION FUNCTIONS                 */
/*******************************************************/

/* Executes instruction described in @inst based on opcode */
void execute_instruction(vm_state* vm, instruction *inst)
{
	switch (inst->opcode) {
		case JSR: op_jsr(vm, inst); break;
		case JMP: op_jmp(vm, inst); break;
		case JPZ: op_jpz(vm, inst); break;
		case JNZ: op_jnz(vm, inst); break;
		case NND: op_nnd(vm, inst); break;
		case DNN: op_dnn(vm, inst); break;
		case INC: op_inc(vm, inst); break;
		case DEC: op_dec(vm, inst); break;
		case DDA: op_dda(vm, inst); break;
		case BUS: op_bus(vm, inst); break;
		case LUM: op_lum(vm, inst); break;
		case VID: op_vid(vm, inst); break;
		case DOM: op_dom(vm, inst); break;
		case SNE: op_sne(vm, inst); break;
		case SGE: op_sge(vm, inst); break;
		case SLE: op_sle(vm, inst); break;
		case ADD: op_add(vm, inst); break;
		case SUB: op_sub(vm, inst); break;
		case MUL: op_mul(vm, inst); break;
		case DIV: op_div(vm, inst); break;
		case MOD: op_mod(vm, inst); break;
		case SEQ: op_seq(vm, inst); break;
		case SLT: op_slt(vm, inst); break;
		case SGT: op_sgt(vm, inst); break;
		case LAA: op_laa(vm, inst); break;
		case LAS: op_las(vm, inst); break;
		case LDA: op_lda(vm, inst); break;
		case STA: op_sta(vm, inst); break;
		case ICH: op_ich(vm, inst); break;
		case OCH: op_och(inst);     break;
		case INU: op_inu(vm, inst); break;
		case ONU: op_onu(inst);     break;
		default:
			exit(EXIT_FAILURE);
	}
}

/* Main loop of VM */
void run(vm_state *vm)
{
	instruction curr_instr;

	while(vm->IP < vm->program_length) {
		fetch_instruction(vm, &curr_instr);

		vm->IP = mod(vm->IP + 1);
		
		load_operand(vm, &curr_instr);
		execute_instruction(vm, &curr_instr);
	}
}

/*******************************************************/
/*          END OF EXECUTION FUNCTIONS                 */
/*******************************************************/

/*******************************************************/
/*                    PARSER FUNCTIONS                 */
/*******************************************************/


/* TODO: liczby oddzielone spacją !!!!! */
void clean_code(char *prog, char *code, int *length)
{
	int len, i,j;
	len = strlen(prog);

	j = 0;
	i = 0;
	while (i < len) {
		char c = prog[i];

		if(c == COMMENT_SYMBOL) {
			while(i < len && prog[i++] != '\n');
		}
		/* Skip white characters */
		else if(c == ' ' || c == '\t' || c == '\n') {
			i++;
		}
		else {
			code[j++] = c;
			i++;
		}
	}
	code[j] = '\0';
	*length = j;
}

OPCODE get_opcode(char *code, int i, int *add_mode_index)
{
	OPCODE op;

	*add_mode_index = i+1;

	switch(code[i]) {
		/* Starting with backslash */
		case '\\':
			switch(code[i+1]) {
				case '_': op = JSR; break;
				case '~': op = JNZ; break;
				case '|': op = DNN; break;	
				case '}': op = INC; break;	
				case '{': op = DEC; break;	
				case '+': op = DDA; break;	
				case '-': op = BUS; break;	
				case '*': op = LUM; break;
				case '/': op = VID; break;	
				case '%': op = DOM; break;	
				case '=': op = SNE; break;	
				case '<': op = SGE; break;
				case '>': op = SLE; break;	
				case '?': op = INU; break;
				case '!': op = ONU; break;
			}
			(*add_mode_index)++;
			break;

		/* Without backslash */
		case '_': op = JMP; break;
		case '~': op = JPZ; break;
		case '|': op = NND; break;
		case '+': op = ADD; break;
		case '-': op = SUB; break;
		case '*': op = MUL; break;
		case '/': op = DIV; break;
		case '%': op = MOD; break;
		case '=': op = SEQ; break;
		case '<': op = SLT; break;
		case '>': op = SGT; break;
		case ',': op = LAA; break;
		case '`': op = LAS; break;
		case '.': op = LDA; break;
		case ':': op = STA; break;	
		case '?': op = ICH; break;	
		case '!': op = OCH; break;		
	}
	return op;
}

ADDRESS_MODE get_address_mode(char *code, int address_index)
{
	ADDRESS_MODE add;
	switch(code[address_index]) {
		case '@': add = acc; break;
		case '^': add = ind; break;
		case '}': add = pop; break;
		case '{': add = psh; break;
		case '#': add = imm; break;
		case '$': add = dis; break;
		case '&': add = rel; break;
		default:
			add = _abs;
	}
	return add;
}

void parse_program(vm_state *vm, char *prog)
{
	char code[CODE_SIZE];
	int labels[30];
	int length;
	int i;
	int j;
	int size_in_memory=0;

	stack use_declare_p_s;  /*  ( \) */
	stack declare_use_p_s;  /* \(  ) */
	stack use_declare_b_s;  /*  [ \] */
	stack declare_use_b_s;  /* \[  ] */

	init_stack(&use_declare_b_s);
	init_stack(&declare_use_b_s);

	init_stack(&use_declare_p_s);
	init_stack(&declare_use_p_s);

	memset(labels, -1, 30 * sizeof(int) );
	clean_code(prog, code, &length);

	i = 0;
	while(i < length) {
		int int_value;
		int bytes;

		/* Use declare brackets stack START */
		if(code[i] == '[') {
			/* For now we don't know what to put here, so push this address to fill later*/
			push_stack(&use_declare_b_s, size_in_memory++);
			i++;
		}
		/* Use declare bracckets stack END */
		else if(code[i] == '\\' && code[i+1] == ']') {
			/* Now we know what to put in gap */
			vm->memory[ pop_stack(&use_declare_b_s) ] = size_in_memory;
			i += 2;
		}
		/* Declare use brackets stack START */
		else if(code[i] == '\\' && code[i+1] == '[') {
			push_stack(&declare_use_b_s, size_in_memory);
			i += 2;
		}
		/* Declare use brackets stack END */
		else if (code[i] == ']') {
			vm->memory[size_in_memory++] = pop_stack(&declare_use_b_s);
			i++;
		}
		/* Use declare parentheses stack START */
		else if(code[i] == '(') {
			/* For now we don't know what to put here, so push this address to fill later*/
			vm->memory[size_in_memory] = -1;
			push_stack(&use_declare_p_s, size_in_memory++);
			i++;
		}
		/* Use declare parentheses stack END */
		else if(code[i] == '\\' && code[i+1] == ')') {
			/* Now we know what to put in gap */
			vm->memory[ pop_stack(&use_declare_p_s) ] = size_in_memory;
			i += 2;
		}
		/* Declare use parentheses stack START */
		else if(code[i] == '\\' && code[i+1] == '(') {
			push_stack(&declare_use_p_s, size_in_memory);
			i += 2;
		}
		/* Declare use parentheses stack END */
		else if (code[i] == ')') {
			vm->memory[size_in_memory++] = pop_stack(&declare_use_p_s);
			i++;
		}
		/* Label declaration */
		else if(code[i] == '\\' && isletter(code[i+1])) {
			labels[code[i+1] - 'a'] = size_in_memory;
			i += 2;
		}
		/* Label usage */
		else if (isletter(code[i])) {
			/* Insert placeholder value */
			vm->memory[size_in_memory++] = 255 * code[i];
			i++;
		}
		/* Array declaration */
		else if(code[i] == '\\' && code[i+1] == '\"') {
			i+= 2;
			scan_int(code + i, &int_value, &bytes);
			/* int_value == size of array */
			/* skip int_value cells */
			size_in_memory += int_value;
			i+= bytes;
		}
		/* String declaration */
		else if(code[i] == '"') {
			i++;
			while(code[i] != '"') {
				/* Escape sequence*/
				if(code[i] == '\\') {
					vm->memory[size_in_memory++] = get_escape_sequence(code[i+1]);
					i += 2;
				}
				else {
					vm->memory[size_in_memory++] = code[i];
					i++;
				}
			}
			i++;
		}
		/* Unsigned int */
		else if( scan_int(code + i, &int_value, &bytes) ) {
			vm->memory[size_in_memory++] = mod(int_value);
			i += bytes;
		}
		/* Signed int */
		else if (code[i] == '\\' && scan_int(code + i + 1, &int_value, &bytes) ) {
			int_value *= -1;
			vm->memory[size_in_memory++] = mod(int_value);
			i += bytes + 1;
		}
		/* Standard operation */
		else {

			OPCODE op;
			ADDRESS_MODE  add;
			int address_index;

			op  = get_opcode(code, i, &address_index);
			add = get_address_mode(code, address_index);

			vm->memory[size_in_memory++] = (op << 3) + add;

			/* We don't want to skip if _abs matched */
			if(add != _abs)
				i = address_index + 1;
			else
				i = address_index;
		}

	}
	/* Insert labels */
	for(j = 0; j < size_in_memory; j++)
		if (vm->memory[j]/255 > 1)
			vm->memory[j] = labels[ vm->memory[j]/255 - 'a' ];
	vm->program_length = size_in_memory;
	
}

/*******************************************************/
/*               END OF PARSER FUNCTIONS               */
/*******************************************************/

/* Sets values of registers and memory to 0 */
void reset_machine(vm_state *vm)
{
	vm->AC = 0;
	vm->SP = 0;
	vm->IP = 0;
	vm->program_length = 0;

	memset(vm->memory, 0, sizeof(int)*N);
}

void load_program(vm_state *vm, char *prog)
{
	/* Program is sequence of numbers */
	if ( isdigit(prog[0]) ) {
		int offset = 0;
		int i = 0;
		int bytes_read;
		int ins;

		while(scan_int(prog + offset, &ins, &bytes_read)) {
			offset += bytes_read;
			vm->memory[i++] = ins;
		}
		vm->program_length = i-1;
	}
	/* Program as a source code */
	else {
		parse_program(vm, prog);
	}
}

int main(int argc, char *argv[])
{
	vm_state vm;
	reset_machine(&vm);

	if(argc > 1)
		load_program(&vm, argv[1]);

	run(&vm);

	return 0;
}
