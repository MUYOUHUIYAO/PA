#include "cpu/exec/template-start.h"
#define instr call


static void do_execute (){
	if(2 == DATA_BYTE){
		reg_w(R_SP) -= 2;
		MEM_W(reg_w(R_SP), (uint16_t)(cpu.eip& 0x0000ffff));

		cpu.eip = (cpu.eip + op_src->val)&0x0000ffff;
	}
	if(4 == DATA_BYTE){
		cpu.esp = cpu.esp-4;
		printf("%x\n",cpu.esp );
		MEM_W(cpu.esp,  cpu.eip);
		cpu.eip = cpu.eip+ op_src->val;
	}

	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"