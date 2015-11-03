#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	switch(ops_decoded.opcode){
		case 97:
			if(CF == 0 && ZF == 0){
				OPERAND_W(op_dest,1);
			}else OPERAND_W(op_dest , 0);
			break;
		default: break;
	}
	/* TODO: Update EFLAGS. */

	print_asm_template2();
}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"
