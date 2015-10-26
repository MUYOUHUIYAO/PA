#include "cpu/exec/template-start.h"
#define instr call

//#include "push-template.h"

static void do_execute (){
	if(4 == DATA_BYTE){
		cpu.esp = cpu.esp-4;
		cpu.ebp = cpu.eip;
		cpu.eip = cpu.eip+ op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"