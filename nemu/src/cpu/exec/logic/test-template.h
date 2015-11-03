#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
	DATA_TYPE  result = op_dest -> val & op_src -> val;

	unset_EFLAGS(E_CF);
	unset_EFLAGS(E_OF);

	/**update EFLAGS***/
	if(result == 0) set_EFLAGS(E_ZF);
	else unset_EFLAGS(E_ZF);

	if(DATA_BYTE == 1 && (result & 0x80) ) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 2 && (result & 0x8000)) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 4 && (result & 0x80000000)) set_EFLAGS(E_SF);
	else unset_EFLAGS(E_SF);

	uint8_t lowByte = result & 0xff;
	if((! (lowByte & 0x01)) ^ (! (lowByte & 0x02) )^ (! (lowByte & 0x04)) ^ (! (lowByte & 0x08) ) ^ (! (lowByte & 0x10)) 
		^ (! (lowByte & 0x20) )^(! (lowByte & 0x40) )^ (! (lowByte & 0x80) )){
		unset_EFLAGS(E_PF);
	}else set_EFLAGS(E_PF);

	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2a)
make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"