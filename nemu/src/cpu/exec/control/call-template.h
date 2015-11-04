#include "cpu/exec/template-start.h"
#define instr call


static void do_execute (){
	if(ops_decoded.opcode == 0xe8 ){
		if(2 == DATA_BYTE){
			cpu.esp =cpu.esp - 2;
			MEM_W(cpu.esp, (uint16_t)(cpu.eip& 0x0000ffff));

			cpu.eip = (cpu.eip + op_src->val)&0x0000ffff;
		}
		else if(4 == DATA_BYTE){
			cpu.esp = cpu.esp-4;
			MEM_W(cpu.esp,  cpu.eip);
			cpu.eip = cpu.eip+ op_src->val;
		}
	}else if(ops_decoded.opcode == 0xff){
		if(2 == DATA_BYTE){
			cpu.esp =cpu.esp - 2;
			MEM_W(cpu.esp, (uint16_t)(cpu.eip& 0x0000ffff));

			cpu.eip = ( op_src->val)&0x0000ffff;
		}
		else if(4 == DATA_BYTE){
			cpu.esp = cpu.esp-4;
			MEM_W(cpu.esp,  cpu.eip);
			cpu.eip =  op_src->val;
		}
	}

	print_asm_template1();
}

make_instr_helper(i)
///#if DATA_BYTE == 2 || DATA_BYTE == 4
//make_instr_helper(rm)
//#endif

#include "cpu/exec/template-end.h"