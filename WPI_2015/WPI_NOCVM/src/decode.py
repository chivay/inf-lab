opcodes = ["JSR",
		   "JMP",
		   "JPZ",
		   "JNZ",
		   "NND",
		   "DNN",
		   "INC",
		   "DEC",
		   "DDA",
		   "BUS",
		   "LUM",
		   "VID",
		   "DOM",
		   "SNE",
		   "SGE",
		   "SLE",
		   "ADD",
		   "SUB",
		   "MUL",
		   "DIV",
		   "MOD",
		   "SEQ",
		   "SLT",
		   "SGT",
		   "LAA",
		   "LAS",
		   "LDA",
		   "STA",
		   "ICH",
		   "OCH",
		   "INU",
		   "ONU"]

address_modes = [ "acc",
				  "ind",
				  "pop",
				  "psh",
				  "imm",
				  "abs",
				  "dis",
				  "rel" ]

x = input()
x = [int(v) for v in x.split(' ')]

for i, instr in enumerate(x):
	opcode = instr//8
	ad_mode = instr % 8
	print('{0:>2}: {1:>3}\t {2} {3}'.format(i, instr, opcodes[opcode], address_modes[ad_mode]))
