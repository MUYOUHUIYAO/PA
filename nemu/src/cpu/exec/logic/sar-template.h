#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	uint8_t temp= count;
	while(temp !=0){
		if(dest & 0x01) set_EFLAGS(E_CF);
		else unset_EFLAGS(E_CF);
		dest =dest >> 1;
		temp -=1;
	}

	OPERAND_W(op_dest, dest);

	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */

	 if(count == 1){
		 unset_EFLAGS(E_OF);
	 }

	 if(dest == 0) set_EFLAGS(E_ZF);
	 else unset_EFLAGS(E_ZF);

	 uint8_t lowByte = dest & 0xff;
	if((! (lowByte & 0x01)) ^ (! (lowByte & 0x02) )^ (! (lowByte & 0x04)) ^ (! (lowByte & 0x08) ) ^ (! (lowByte & 0x10)) 
		^ (! (lowByte & 0x20) )^(! (lowByte & 0x40) )^ (! (lowByte & 0x80) )){
		unset_EFLAGS(E_PF);	//偶数个0
	}else set_EFLAGS(E_PF);

	if(DATA_BYTE == 1 && (dest & 0x80) ) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 2 && (dest & 0x8000)) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 4 && (dest & 0x80000000)) set_EFLAGS(E_SF);
	else unset_EFLAGS(E_SF);
	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
