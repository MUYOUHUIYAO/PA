#include "cpu/exec/template-start.h"
#define instr jmp


static void do_execute (){
	printf("eip = 0x%08x\t", cpu.eip);
	printf("rel = 0x%08x\t", op_src -> val );
	cpu.eip += op_src->val;
	if(DATA_BYTE == 2){
		cpu.eip = cpu.eip & 0x0000ffff ;
	}
	printf("dest_eip = 0x%08x\n", cpu.eip );
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