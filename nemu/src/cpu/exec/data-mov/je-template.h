#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
	if(ZF == 0){
		cpu.eip+= (uint32_t) (op_src->val);//printf("%x\n",op_src->val );
		if(DATA_BYTE == 2){
			cpu.eip = cpu.eip & 0x0000ffff;
		}	
	}
	/**update EFLAGS***/
	print_asm_template1();
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"