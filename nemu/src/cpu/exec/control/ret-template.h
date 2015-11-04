#include "cpu/exec/template-start.h"
#define instr ret


static void do_execute (){
	if(ops_decoded.opcode == 0xc2){
		if(DATA_BYTE == 2){
			cpu.eip = MEM_R(cpu.esp);
			cpu.esp += 2;
			cpu.eip &= 0x0000ffff;
		}else{
			cpu.eip = MEM_R(cpu.esp);
			cpu.esp += 4;
		}
	}
	cpu.esp += op_src ->val;
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"