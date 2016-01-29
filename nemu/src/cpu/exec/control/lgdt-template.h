#include "cpu/exec/template-start.h"
#define instr lgdt


static void do_execute (){
	uint16_t limit = swaddr_read(op_src -> val, 2);
	uint32_t base = swaddr_read(op_src -> val + 2, 4);
#if DATA_BYTE == 2
	base = base & 0x00ffffff;
#endif
	cpu.GDTR.limit = limit;
	cpu.GDTR.base = base;
	insertSegDesc(0x0, 0xfffff);
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(m)
#endif

#include "cpu/exec/template-end.h"