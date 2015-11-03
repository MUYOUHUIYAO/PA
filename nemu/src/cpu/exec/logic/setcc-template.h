#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	int condition = 0;
	switch(ops_decoded.opcode & 0x0ff){
		case 0x97: condition = (CF ==0 && ZF == 0);break;
		case 0x93: condition = (CF == 0);break;
		case 0x92: condition = (CF == 1); break;
		case 0x96: condition = (CF == 1 || ZF == 1); break;
		case 0x94: condition = (ZF == 1); break;
		case 0x9f: condition = (ZF == 0 || SF == OF); break;
		case 0x9d: condition =(SF ==OF);break;
		case 0x9c: condition = (SF != OF); break;
		case 0x9e: condition = (ZF == 1 && SF != OF); break;
		case 0x95: condition = (ZF == 0); break;
		case 0x91: condition = (OF ==0); break;
		case 0x9b: condition = (PF == 0); break;
		case 0x99: condition = (SF ==0 );break;
		case 0x90: condition = (OF == 0); break;
		case 0x9a: condition = (PF==1); break;
		case 0x98: condition = (SF == 1); break;
		default: break;
	}
	OPERAND_W(op_dest , condition);
	/* TODO: Update EFLAGS. */

	print_asm_template2();
}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"
