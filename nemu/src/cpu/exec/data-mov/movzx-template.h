#include "cpu/exec/template-start.h"
#define instr movzx

static void do_execute (){
	uint32_t zeroExtend = 0;
	if(DATA_BYTE == 1)
		zeroExtend = 0x000000ff & op_src->val;
	else if(DATA_BYTE == 2)
		zeroExtend = 0x0000ffff & op_src->val;
	OPERAND_W(op_dest , zeroExtend);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"