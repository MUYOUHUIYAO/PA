#include "cpu/exec/template-start.h"

#define instr cmov

static void do_execute(){
	uint8_t condition=0;

	switch(ops_decoded.opcode & 0x0ff){
		case 0x47: condition = (CF == 0 && ZF == 0); break;			//CMOVA, CMOVNBE
		case 0x43: condition = (CF == 0); break;					//CMOVAE, CMOVNB, CMOVNC
		case 0x42: condition = (CF == 1); break;					//CMOVB, CMOVC, CMOVNAE
		case 0x46: condition = (CF == 1 || ZF == 1); break;			//CMOVBE, CMOVNA
		case 0x44: condition = (ZF == 1); break;					//CMOVE, CMOVZ
		case 0x4f: condition = (ZF == 0 && SF == OF); break;			//CMOVG, CMOVNLE
		case 0x4d: condition = (SF == OF); break;					//CMOVGE, CMOVNL
		case 0x4c: condition = (SF !=OF); break;					//CMOVL, CMOVNGE
		case 0x4e: condition = (ZF == 1 || SF != OF); break;			//CMOVLE, CMOVNG
		case 0x45: condition = (ZF == 0); break;					//CMOVNE, CMOVNZ
		case 0x41: condition = (OF == 0); break;					//CMOVNO
		case 0x4b: condition = (PF == 0); break;					//CMOVNP, CMOVPO
		case 0x49: condition = (SF == 0); break;					//CMOVNS
		case 0x40: condition = (OF == 1); break;					//CMOVO
		case 0x4a: condition = (PF == 1); break;					//CMOVP, CMOVpe
		case 0x48: condition = (SF == 1); break;					//CMOVS

	}

	if(condition){
		OPERAND_W(op_dest, op_src->val);
	}

	/**update EFLAGS***/
	print_asm_template2();
}

make_instr_helper(rm2r);

#include "cpu/exec/template-end.h"
