#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE result = op_dest->val - op_src->val;

	if(result == 0) set_EFLAGS(E_ZF);
	else unset_EFLAGS(E_ZF);

	if((uint32_t)(op_dest->val) < (uint32_t)(op_src -> val)) set_EFLAGS(E_CF);
	else unset_EFLAGS(E_CF);

	uint8_t lowByte = result & 0xff;
	if((! (lowByte & 0x01)) ^ (! (lowByte & 0x02) )^ (! (lowByte & 0x04)) ^ (! (lowByte & 0x08) ) ^ (! (lowByte & 0x10)) 
		^ (! (lowByte & 0x20) )^(! (lowByte & 0x40) )^ (! (lowByte & 0x80) )){
		unset_EFLAGS(E_PF);
	}else set_EFLAGS(E_PF);

	if(DATA_BYTE == 1) {
		if( (op_dest->val & 0x0f) < (op_src -> val & 0x0f)) set_EFLAGS(E_AF);
	}else unset_EFLAGS(E_AF);

	if(DATA_BYTE == 1 && (result & 0x80) ) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 2 && (result & 0x8000)) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 4 && (result & 0x80000000)) set_EFLAGS(E_SF);
	else unset_EFLAGS(E_SF);

	//if(result + op_src->val != op_dest->val) set_EFLAGS(E_OF);
	//else unset_EFLAGS(E_OF);

	if( (1==DATA_BYTE && (result & 0xff) + (op_src->val & 0xff) != (op_dest->val & 0xff))) set_EFLAGS(E_OF);
	else if( (2==DATA_BYTE && (result & 0xffff) + (op_src->val & 0xffff) != (op_dest->val & 0xffff)))set_EFLAGS(E_OF);
	else if( (4==DATA_BYTE && (result & 0xffffffff) + (op_src->val & 0xffffffff) != (op_dest->val & 0xffffffff))) set_EFLAGS(E_OF);
	else unset_EFLAGS(E_OF);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"