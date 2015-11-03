#include "cpu/exec/template-start.h"
#define instr jbe


static void do_execute (){
	if(CF == 1 || ZF == 1){
		if(DATA_BYTE == 1)
			cpu.eip += (int32_t)((int8_t)op_src->val);
		if(DATA_BYTE == 2){
			cpu.eip += (int32_t)((int8_t)op_src->val);
			cpu.eip = cpu.eip & 0x0000ffff;
		}
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"