#include "template-start.h"

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
	if((! lowByte & 0x01) ^ (! lowByte & 0x02) ^ (! lowByte & 0x04) ^ (! lowByte & 0x10) 
		^ (!lowByte & 0x20) ^(!lowByte & 0x40) ^ (! lowByte & 0x80)){
		set_EFLAGS(E_PF);
	}else unset_EFLAGS(E_PF);
}

make_instr_help(i2rm)

#include "template-end.h"