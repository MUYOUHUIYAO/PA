#include "cpu/exec/template-start.h"
#define instr movsx

static void do_execute (){
	DATA_TYPE signExtend;
	if(DATA_BYTE == 1)
		signExtend = (DATA_TYPE)((int8_t) op_src->val);
	else
		signExtend = (DATA_TYPE)((int16_t) op_src->val);
	OPERAND_W(op_dest , signExtend);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"