#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest,result);
	
	/* update EFLAGS  */
	if(result == 0) set_EFLAGS(E_ZF);
	else unset_EFLAGS(E_ZF);

	if((DATA_TYPE)(op_dest->val) < (DATA_TYPE)(op_src -> val)) set_EFLAGS(E_CF);
	else unset_EFLAGS(E_CF);

	uint8_t lowByte = result & 0xff;
	if((! (lowByte & 0x01)) ^ (! (lowByte & 0x02) )^ (! (lowByte & 0x04)) ^ (! (lowByte & 0x10)) 
		^ (! (lowByte & 0x20) )^(! (lowByte & 0x40) )^ (! (lowByte & 0x80) )){
		set_EFLAGS(E_PF);
	}else unset_EFLAGS(E_PF);

	if(DATA_BYTE == 1) {
		if( (op_dest->val & 0x0f) < (op_src -> val & 0x0f)) set_EFLAGS(E_AF);
	}else unset_EFLAGS(E_AF);

	if(DATA_BYTE == 1 && (result & 0x80) ) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 2 && (result & 0x8000)) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 4 && (result & 0x800000)) set_EFLAGS(E_SF);
	else unset_EFLAGS(E_SF);

	if(result + op_src->val != op_dest->val) set_EFLAGS(E_OF);
	else unset_EFLAGS(E_OF);
	
	print_asm_template2();
}

make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"