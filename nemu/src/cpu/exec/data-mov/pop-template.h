#include "cpu/exec/template-start.h"
#define instr pop

static void do_execute (){
	if(DATA_BYTE == 2){
		OPERAND_W(op_dest, MEM_R(cpu.esp)); 
		cpu.esp = cpu.esp+2;
	}
	else if(DATA_BYTE == 4){
		OPERAND_W(op_dest, MEM_R(cpu.esp));
		printf("%s\t%x\n",op_src2->str, cpu.esp );
		cpu.esp = cpu.esp+4;
	}
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(m)
#endif

#include "cpu/exec/template-end.h"