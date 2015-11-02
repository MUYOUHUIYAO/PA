#include "cpu/exec/template-start.h"
#define instr movzx

static void do_execute (){
	DATA_TYPE signExtend = 0;
	if(DATA_BYTE == 1)
		signExtend = (DATA_TYPE)((uint8_t) op_src->val);
	else if(DATA_BYTE == 2)
		signExtend = (DATA_TYPE)((uint16_t) op_src->val);
	OPERAND_W(op_dest , signExtend);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"