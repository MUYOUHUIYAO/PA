#include "cpu/exec/template-start.h"
#define instr jmp


static void do_execute (){
	if(DATA_BYTE == 1)
		cpu.eip += (uint32_t)(int32_t)(int8_t)op_src->val;
	else if(DATA_BYTE == 2)
		cpu.eip += (uint32_t)(int32_t)(int16_t)op_src->val;
	else
		cpu.eip += op_src->val;
	if(DATA_BYTE == 2){
		cpu.eip = cpu.eip & 0x0000ffff ;
	}
	print_asm_template1();
}

make_instr_helper(i)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_rm_, SUFFIX)){
	concat(decode_rm_, SUFFIX)(eip + 1);
	if(2 == DATA_BYTE){
		cpu.eip = ( op_src->val)&0x0000ffff;
	}
	else if(4 == DATA_BYTE){
		cpu.eip =  op_src->val;
	}
	return 0;	
}
#endif

#include "cpu/exec/template-end.h"