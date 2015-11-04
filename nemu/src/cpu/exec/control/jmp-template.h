#include "cpu/exec/template-start.h"
#define instr jmp


static void do_execute (){
	uint32_t op = ops_decoded.opcode;
	if(op == 0xeb || op == 0xe9 ){
		cpu.eip += op_src->val;
		if(DATA_BYTE == 2){
			cpu.eip = cpu.eip && 0x0000ffff ;
		}
	}else if(op == 0xff){
		printf("%x\n",DATA_BYTE );
		if(2 == DATA_BYTE){
			cpu.eip = ( op_src->val)&0x0000ffff;
		}
		else if(4 == DATA_BYTE){
			cpu.eip =  op_src->val;
		}
	}

	print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"