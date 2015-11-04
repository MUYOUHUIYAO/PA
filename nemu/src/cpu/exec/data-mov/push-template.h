#include "cpu/exec/template-start.h"
#define instr push

static void do_execute (){
	if(DATA_BYTE == 2){
		cpu.esp = cpu.esp-2;
		//实现段寄存器
		MEM_W(cpu.esp, op_src->val);
	}
	else{
		cpu.esp = cpu.esp-4;
		//实现段寄存器
		MEM_W(cpu.esp, op_src->val);
	}
	print_asm_template1();
}

make_instr_helper(i)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(m)
#endif

#include "cpu/exec/template-end.h"