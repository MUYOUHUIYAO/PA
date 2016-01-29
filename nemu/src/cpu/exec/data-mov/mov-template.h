#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4

make_helper(mov_r2cr_l){
	int len = decode_r2rm_l(eip + 1);
	cpu.cr0.val = op_src->val;

	print_asm("movl %s, %%cr0", op_src -> str);
	return len+1;
}

make_helper(mov_cr2r_l){
	int len = decode_rm2r_l(eip + 1);
	OPERAND_W(op_dest, cpu.cr0.val);

	print_asm("movl %%cr0, %s", op_dest -> str);
	return len+1;

}

#endif

#if DATA_BYTE == 2
make_helper(mov_rm2sreg_w){
	decode_rm2r_w(eip + 1);

	uint8_t op = instr_fetch(eip + 1, 1);
	printf("0x%x\n", (op >> 3) & 0x7);
	switch((op >> 3) & 0x7){
		case 0: cpu.ES.val = op_src->val;
			print_asm("mov %s, %%es", op_src -> str);
			break;			//es
		case 1: cpu.CS.val = op_src -> val;
			print_asm("mov %s, %%es", op_src -> str);
			break;			//cs
		case 2: cpu.SS.val = op_src -> val;
			print_asm("mov %s, %%es", op_src -> str);
			break;			//ss
		case 3: cpu.DS.val = op_src -> val;
			print_asm("mov %s, %%es", op_src -> str);
			break;			//ds
	}
	return 2;
}
#endif


#include "cpu/exec/template-end.h"
